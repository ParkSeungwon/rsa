#pragma once
#include<functional>
#include<mutex>
#include<atomic>
#include<thread>
#include<vector>
#include<future>
#include<condition_variable>
#include<cstdlib>

struct Any
{
	void *data;
	template <typename T> Any(T n) {
		data = malloc(sizeof(T));
		*(T*)data = n;
	}
	template <typename T> operator T() {
		return *(T*)data;
	}
	~Any() {
		if(data) {
			free(data);
			data = nullptr;
		}
	}
};


class AutoThread
{//automatically manages threads considering cpu core
public:
	AutoThread() {
		cpu = std::thread::hardware_concurrency();
	}
	template <typename F> auto add_thread(F f) {//use bind to pass argument
		auto prom = std::make_shared<std::promise<Any>>();
		v.push_back({std::thread(&AutoThread::wrap<F>, this, f, std::ref(*prom)), 
				prom});//?????
		return prom->get_future();
	}
	~AutoThread() {
		for(auto& a : v) {
			a.first.join();
		}
	}

private:
	std::vector<std::pair<std::thread, std::shared_ptr<std::promise<Any>>>> v;
	std::mutex mtx;
	int cpu;
	std::condition_variable cv;
	std::atomic<int> using_cpu{0};
	template<typename F> void wrap(F f, std::promise<Any>& prom) {
		std::unique_lock<std::mutex> lck{mtx};
		while(using_cpu >= cpu) cv.wait(lck);
		if(using_cpu < cpu-1) {
			using_cpu++;
//			std::cout << "using_cpu : " << using_cpu << std::endl;
			lck.unlock();
			cv.notify_one();
			prom.set_value(f());
			using_cpu--;
		} else {
			using_cpu++;
//			std::cout << "using_cpu : " << using_cpu << std::endl;
			prom.set_value(f());
			using_cpu--;
			lck.unlock();
			cv.notify_one();
		}
	}
};

