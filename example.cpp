#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.h"

int man_num = 0;
std::mutex man_num_mutex;

class A
{
private:
    std::mutex data_mutex;

public:
    int data = 0;

    void add_data(int num)
    {
        for (int i = 0; i < num; i++)
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            data++;
        }
    }

    void sub_data(int num)
    {
        for (int i = 0; i < num; i++)
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            data--;
        }
    }
};

int main()
{
    A a;
    ThreadPool pool(5);
    std::vector<std::future<void>> results;
    
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue(
                &A::add_data, &a, 100000
            )
        );
    }
    results.emplace_back(
        pool.enqueue(
            &A::sub_data, &a, 100000
        )
    );

    for(auto && result: results)
        result.get();
    std::cout << "a.data: " << a.data << std::endl;
    
    return 0;
}



// int main()
int main01()
{
    
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    
    return 0;
}

// int main()
int main02()
{
    ThreadPool pool(5);

#if 0
    std::vector< std::future<int> > results;
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }
    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
#endif

#if 1
    std::vector< std::future<int> > results;
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                for (int j = 0; j < 100000; j++)
                {
                    std::lock_guard<std::mutex> lock(man_num_mutex);
                    // man_num_mutex.lock();
                    man_num++;
                    // man_num_mutex.unlock();
                }
                return i*i;
            })
        );
    }
    for(auto && result: results)
        result.get();//显式地阻塞主线程，否则无法保证下一步就执行完其他线程（和demo02不一样）
    // std::cout << std::endl;
    std::cout << "man_num: " << man_num << std::endl;
#endif

    return 0;
}



// int main()
int demo01()
{
    int num = 0;
    std::mutex num_mutex;
    auto my_thread = [&]() {
        for (int i = 0; i < 100000; i++)
        {
            std::lock_guard<std::mutex> lock();
            num++;
        }
        return 114514;
    };

    auto my_thread1 = [&]() {
        for (int i = 0; i < 100000; i++)
        {
            std::lock_guard<std::mutex> lock();
            num--;
        }
        return 1919;
    };
    std::packaged_task<int(void)> mypt(my_thread);
    std::packaged_task<int(void)> mypt1(my_thread);
    std::future<int> result = mypt.get_future();
    std::future<int> result1 = mypt1.get_future();

    std::packaged_task<int(void)> mypt2(my_thread1);
    std::future<int> result2 = mypt2.get_future();

#if 1
    mypt();
    mypt1();
    mypt2();
#endif

#if 1
    // result.get();
    // result1.get();
    // result2.get();
#endif
    std::cout << "num: " << num << std::endl;
    return 1;
}

// int main()
int demo02()
{
    int num = 0;
    std::mutex num_mutex;
    auto my_thread = [&]() {
        for (int i = 0; i < 100000; i++)
        {
            std::lock_guard<std::mutex> lock();
            num++;
        }
        return 114514;
    };

    auto my_thread1 = [&]() {
        for (int i = 0; i < 100000; i++)
        {
            std::lock_guard<std::mutex> lock();
            num--;
        }
        return 1919;
    };
    std::packaged_task<int(void)> mypt(my_thread);
    std::packaged_task<int(void)> mypt1(my_thread);
    std::future<int> result = mypt.get_future();
    std::future<int> result1 = mypt1.get_future();

    std::packaged_task<int(void)> mypt2(my_thread1);
    std::future<int> result2 = mypt2.get_future();

#if 1
    mypt();
    mypt1();
    mypt2();
#endif

#if 1
    // result.get();
    // result1.get();
    // result2.get();
#endif
    std::cout << "num: " << num << std::endl;
    return 1;
}
