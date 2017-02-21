#include<functional>
#include<mutex>
#include<atomic>
#include<thread>
#include<vector>
#include<future>
#include<condition_variable>

class AutoThread
{//automatic thread considering cpu core
public:
	AutoThread() {
		cpu = std::thread::hardware_concurrency();
	}
	template <typename F> auto add_thread(F f) {//use bind to pass argument
		typedef typename std::result_of<F&()>::type R;
		auto* prom = new std::promise<R>;
		vt.push_back(std::thread(&AutoThread::wrap<F,R>, this, f, std::ref(*prom)));
		vp.push_back((std::promise<void>*)prom);//?????
		//std::declval<R>().value();
		return prom->get_future();
	}
	~AutoThread() {
		for(auto& a : vt) a.join();
		for(auto& a : vp) delete a;
	}
	void spare_cpu(int n) {
		cpu -= n;
	}

private:
	std::vector<std::thread> vt;
	std::vector<std::promise<void>*> vp;
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
			lck.unlock();
			cv.notify_one();
			prom.set_value(f());
			using_cpu--;
		} else {
			using_cpu++;
			prom.set_value(f());
			using_cpu--;
			lck.unlock();
			cv.notify_one();
		}
	}
};

