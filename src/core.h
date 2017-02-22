#include<functional>
#include<mutex>
#include<atomic>
#include<thread>
#include<vector>
#include<future>
#include<condition_variable>
#include<typeindex>

class Base 
{
public:
	virtual void f(){};
};

template <typename T> class Promise : public Base
{
public:
	std::promise<T> prom;
	virtual std::future<T> get_future() {
		return prom.get_future();
	}
	virtual void set_value(T n) {
		prom.set_value(n);
	}
};

class AutoThread
{//automatic thread considering cpu core
public:
	AutoThread() {
		cpu = std::thread::hardware_concurrency();
	}
	template <typename F> auto add_thread(F f) {//use bind to pass argument
		typedef typename std::result_of<F&()>::type R;
		Base* prom = new Promise<R>;
		vt.push_back(std::thread(&AutoThread::wrap<F,R>, this, f, prom));
		vp.push_back(prom);//?????
		//std::declval<R>().value();
		return dynamic_cast<Promise<R>*>(prom)->get_future();
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
	std::vector<Base*> vp;
	std::mutex mtx;
	int cpu;
	std::condition_variable cv;
	std::atomic<int> using_cpu{0};
	template<typename F, typename R = typename std::result_of<F&()>::type> 
	void wrap(F f, Base* prom) {
		std::unique_lock<std::mutex> lck{mtx};
		while(using_cpu >= cpu) cv.wait(lck);
		if(using_cpu < cpu-1) {
			using_cpu++;
			lck.unlock();
			cv.notify_one();
			dynamic_cast<Promise<R>*>(prom)->set_value(f());
			using_cpu--;
		} else {
			using_cpu++;
			dynamic_cast<Promise<R>*>(prom)->set_value(f());
			using_cpu--;
			lck.unlock();
			cv.notify_one();
		}
	}
};

