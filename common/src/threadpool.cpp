#include "common/threadpool.hpp"

#include <cassert>

namespace
{
	unsigned int decideNumberOfThreads(std::optional<unsigned int> numThreads)
	{
		if (numThreads)
			return *numThreads;

		const auto count = std::thread::hardware_concurrency();
		if (count == 0)
			return 1;

		return count;
	}
}

namespace textoolkit
{
	Threadpool::Threadpool(std::optional<unsigned int> numThreads)
		: workers(decideNumberOfThreads(numThreads))
	{
	}

	void Threadpool::start()
	{
		for (auto& worker : this->workers)
		{
			worker.thread = std::thread(&Threadpool::work, this, &worker);
		}
	}

	void Threadpool::abort()
	{
		this->aborted = true;
		decltype(this->tasks)().swap(this->tasks);
		for (auto& worker : this->workers)
		{
			this->tasks.push([]() {});
			this->waitcv.notify_one();
		}
	}

	void Threadpool::enqueue(Task&& task)
	{
		std::lock_guard lck(this->mutex);
		this->tasks.push(task);
		this->workercv.notify_one();
	}

	void Threadpool::waitForAll()
	{
		assert(this->isStarted() && "Not working - probably didn't call start()");
		std::mutex waitmutex;
		std::unique_lock<std::mutex> lck(waitmutex);
		while (true)
		{
			this->waitcv.wait(lck);
			if (!this->isWorking())
				break;
		}
		for (auto& worker : this->workers)
		{
			worker.thread.join();
		}
	}

	bool Threadpool::isStarted() const
	{
		return !this->workers.empty() && this->workers.front().thread.joinable();
	}

	bool Threadpool::isWorking() const
	{
		for (auto& worker : this->workers)
		{
			if (worker.working)
				return true;
		}
		return false;
	}

	Threadpool::ProducerLock Threadpool::lock()
	{
		return ProducerLock(this->producermutex);
	}

	Threadpool::Task Threadpool::getTask()
	{
		while (this->tasks.empty())
		{
			std::unique_lock<std::mutex> lck(this->mutex);
			this->workercv.wait(lck);
		}
		auto& task = this->tasks.front();
		this->tasks.pop();
		return task;
	}

	void Threadpool::work(Worker* worker)
	{
		while(!this->aborted)
		{
			if (auto task = this->getTask())
				task();
		}
		worker->working = false;
		this->waitcv.notify_one();
	}

	// Finite threadpool

	FiniteThreadpool::FiniteThreadpool(std::optional<unsigned int> numThreads, ProgressNotifier progressNotifier)
		: Threadpool(numThreads)
		, progressNotifier(progressNotifier)
	{
	}

	FiniteThreadpool::FiniteThreadpool(ProgressNotifier progressNotifier)
		: Threadpool()
		, progressNotifier(progressNotifier)
	{
	}

	FiniteThreadpool::Task FiniteThreadpool::getTask()
	{
		std::lock_guard<std::mutex> lck(this->mutex);
		if (this->tasks.empty())
			return {};
		auto task = std::move(this->tasks.front());
		this->tasks.pop();
		return task;
	}

	void FiniteThreadpool::work(Worker* worker)
	{
		unsigned int totalTasks = this->tasks.size();
		while (!this->aborted)
		{
			if (auto task = this->getTask())
			{
				task();
				if (this->progressNotifier)
				{
					std::lock_guard<std::mutex> lck(this->mutex);
					this->progressNotifier(totalTasks, totalTasks - this->tasks.size());
				}
			}
			else
				break;
		}
		worker->working = false;
		this->waitcv.notify_one();
	}

	// Composite progress notifier

	CompositeProgressNotifier::CompositeProgressNotifier(ProgressNotifier progressNotifier)
		: progressNotifier(progressNotifier)
	{
	}

	void CompositeProgressNotifier::operator()(unsigned int total, unsigned int completed)
	{
		std::lock_guard<std::mutex> lck(this->mutex);
		auto threadid = std::this_thread::get_id();
		this->status[threadid] = {total, completed};
		unsigned int allTotal = 0;
		unsigned int allCompleted = 0;
		for (auto& [id, status] : this->status)
		{
			allTotal += status.first;
			allCompleted += status.second;
		}
		this->progressNotifier(allTotal, allCompleted);
	}

	ProgressNotifier CompositeProgressNotifier::getFunctor()
	{
		return [this](unsigned int total, unsigned int completed) {
			(*this)(total, completed);
		};
	}
}