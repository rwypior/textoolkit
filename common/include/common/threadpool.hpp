#ifndef _h_textoolkit_common_threadpool
#define _h_textoolkit_common_threadpool

#define _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR 1

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <optional>
#include <unordered_map>

namespace textoolkit
{
	using ProgressNotifier = std::function<void(unsigned int total, unsigned int completed)>;

	// TODO - maybe make this PIMPL at some point
	class Threadpool
	{
	protected:
		struct Worker
		{
			std::thread thread;
			bool working = true;
		};

	public:
		using Task = std::function<void()>;

		struct ProducerLock
		{
			std::lock_guard<std::mutex> guard;

			ProducerLock(std::mutex& mutex)
				: guard(mutex)
			{
			}
		};

	public:
		Threadpool(std::optional<unsigned int> numThreads = {});
		void start();
		void abort();
		void enqueue(Task&& task);
		void waitForAll();
		bool isStarted() const;
		bool isWorking() const;
		[[nodiscard]] ProducerLock lock();

	protected:
		virtual Task getTask();
		virtual void work(Worker* worker);

		std::vector<Worker> workers;
		std::mutex mutex;
		std::mutex producermutex;
		std::condition_variable workercv;
		std::condition_variable waitcv;
		std::queue<Task> tasks;
		unsigned int numFinished = 0;
		bool aborted = false;
	};

	/// This threadpool variant will work as long as tasks are available, after
	/// tasks run out, threadpool finishes it works and waitForAll() exits
	class FiniteThreadpool : public Threadpool
	{
	public:
		FiniteThreadpool(std::optional<unsigned int> numThreads, ProgressNotifier progressNotifier = {});
		FiniteThreadpool(ProgressNotifier progressNotifier = {});

	protected:
		virtual Task getTask() override;
		virtual void work(Worker* worker) override;

	private:
		ProgressNotifier progressNotifier = {};
	};

	class CompositeProgressNotifier
	{
	public:
		CompositeProgressNotifier(ProgressNotifier progressNotifier);
		void operator()(unsigned int total, unsigned int completed);
		ProgressNotifier getFunctor();

	private:
		std::mutex mutex;
		ProgressNotifier progressNotifier;
		std::unordered_map<std::thread::id, std::pair<unsigned int /*total*/, unsigned int /*completed*/>> status;
	};
}

#endif