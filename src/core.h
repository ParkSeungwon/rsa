#include<memory>
#include<functional>
#include<mutex>
#include<atomic>
#include<thread>
#include<vector>
#include<future>
#include<condition_variable>

class AutoThread
{
public:
	AutoThread() {
		cpu = std::thread::hardware_concurrency();
	}
	template <typename F> auto add_thread(F f) {//use bind to pass argument
		auto* prom = new std::promise<typename std::result_of<F&()>::type>;
		v.push_back({std::thread(&AutoThread::wrap<F, typename std::result_of<F&()>::type>, this, f, std::ref(*prom)), (std::promise<void>*)prom});
		auto fut = prom->get_future();
		return fut;
	}
	~AutoThread() {
		for(auto& a : v) {
			if(a.first.joinable()) a.first.join();
			delete a.second;
		}
	}

private:
	std::vector<std::pair<std::thread, std::promise<void>*>> v;
	std::mutex mtx;
	int cpu;
	std::condition_variable cv;
	std::atomic<int> using_cpu{0};
	template<typename F, typename R = typename std::result_of<F&()>::type> 
	void wrap(F f, std::promise<R>& prom) {
		std::unique_lock<std::mutex> lck{mtx};
		while(using_cpu >= cpu) cv.wait(lck);
		if(using_cpu < cpu-1) {
			using_cpu++;
			std::cout << "using_cpu : " << using_cpu << std::endl;
			lck.unlock();
			cv.notify_one();
			prom.set_value(f());
			using_cpu--;
		} else {
			using_cpu++;
			std::cout << "using_cpu : " << using_cpu << std::endl;
			prom.set_value(f());
			using_cpu--;
			lck.unlock();
			cv.notify_one();
		}
	}
};

