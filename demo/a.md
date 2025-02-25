### **1. C++ 线程库概述**

C++11 引入了 `<thread>` 头文件，并通过 `thread` 类提供了线程的创建和管理功能。通过 `mutex` 和 `condition_variable` 等同步机制，可以避免线程间的竞争问题。C++11 的线程库让多线程编程变得更加容易和安全。

### **2. 线程创建与管理**

#### **线程创建：`thread`**

`thread` 类用于创建并启动线程。线程的执行通过将可执行的函数（或可调用对象）传递给线程对象来实现。

**基本使用：**
```cpp
#include <iostream>
#include <thread>

void thread_function() {
    cout << "Hello from thread!" << endl;
}

int main() {
    thread t(thread_function);  // 创建并启动线程
    t.join();  // 等待线程执行完成
    return 0;
}
```
- `thread t(thread_function)`：创建并启动线程 `t`，并让其执行 `thread_function`。
- `t.join()`：等待线程 `t` 完成，阻塞主线程，直到 `t` 执行结束。

**注意**：如果没有调用 `join()` 或 `detach()`，线程在主程序退出时会被强制终止，可能导致未定义行为。

#### **线程返回值：`thread::get_id()`**

可以通过 `thread::get_id()` 获取当前线程的 ID，通常用来调试或区分多个线程。

```cpp
#include <iostream>
#include <thread>

void thread_function() {
    cout << "Thread ID: " << this_thread::get_id() << endl;
}

int main() {
    thread t(thread_function);
    t.join();
    return 0;
}
```

### **3. 线程同步**

多线程中多个线程可能同时访问共享资源，可能导致数据竞态。为了防止这种情况，C++ 提供了几种同步机制。

#### **互斥锁：`mutex`**

互斥锁（mutex）用来保护共享资源，确保同一时刻只有一个线程可以访问共享资源。

**使用方法：**
```cpp
#include <iostream>
#include <thread>
#include <mutex>

mutex mtx;  // 定义互斥锁
int counter = 0;

void increment() {
    lock_guard<mutex> lock(mtx);  // 自动加锁
    ++counter;  // 临界区，只有一个线程能访问
}

int main() {
    thread t1(increment);
    thread t2(increment);
    t1.join();
    t2.join();
    cout << "Counter: " << counter << endl;  // 输出 2
    return 0;
}
```
- `lock_guard<mutex> lock(mtx)`：通过 `lock_guard` 在临界区内自动加锁和解锁。
- `mutex mtx`：定义一个互斥锁来保护共享资源。

#### **条件变量：`condition_variable`**

条件变量用来协调多个线程的执行，它允许线程等待某个条件的发生，并且当条件发生时被唤醒。

**使用方法：**
```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

mutex mtx;
condition_variable cv;
bool ready = false;

void print_id(int id) {
    unique_lock<mutex> lck(mtx);
    while (!ready) cv.wait(lck);  // 等待条件变量
    cout << "Thread " << id << endl;
}

void go() {
    unique_lock<mutex> lck(mtx);
    ready = true;  // 设置条件变量的状态
    cv.notify_all();  // 唤醒所有等待的线程
}

int main() {
    thread threads[10];
    for (int i = 0; i < 10; ++i)
        threads[i] = thread(print_id, i);

    cout << "Preparing to start threads..." << endl;
    go();  // 修改 ready 的状态并通知线程

    for (auto& th : threads) th.join();
    return 0;
}
```
- `condition_variable cv`：定义条件变量，用于线程间同步。
- `cv.wait(lck)`：线程等待条件变量发出的信号。
- `cv.notify_all()`：通知所有等待的线程。

### **4. 线程间通信**

线程间的通信通常通过共享内存、消息队列或条件变量来实现。下面是一个简单的 **生产者-消费者** 模型，展示了如何通过条件变量同步线程。

#### **生产者-消费者：使用 `queue` 和 `condition_variable`**
```cpp
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

queue<int> q;
mutex mtx;
condition_variable cv;
const unsigned MAX_QUEUE_SIZE = 10;

void producer() {
    int i = 0;
    while (true) {
        unique_lock<mutex> lock(mtx);
        while (q.size() == MAX_QUEUE_SIZE) {
            cv.wait(lock);  // 队列满，生产者等待
        }
        q.push(i++);
        cout << "Produced: " << i << endl;
        cv.notify_all();  // 唤醒消费者
    }
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        while (q.empty()) {
            cv.wait(lock);  // 队列空，消费者等待
        }
        int item = q.front();
        q.pop();
        cout << "Consumed: " << item << endl;
        cv.notify_all();  // 唤醒生产者
    }
}

int main() {
    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}
```
- `queue<int> q`：队列用于存储生产的数据。
- 生产者线程负责将数据插入队列，消费者线程负责从队列中取出数据并处理。
- 使用条件变量同步生产者和消费者的执行，防止竞争和过度等待。

### **5. 线程的生命周期与销毁**

线程的生命周期管理是多线程编程中的重要方面。C++ 提供了以下几种方法来管理线程的生命周期：

#### **`join()` 与 `detach()`**

- `join()`：主线程会阻塞，直到被 `join()` 的线程完成。
- `detach()`：将线程与主线程分离，线程开始在后台独立运行。

**示例：**
```cpp
#include <iostream>
#include <thread>

void hello() {
    cout << "Hello from thread!" << endl;
}

int main() {
    thread t(hello);
    t.detach();  // 将线程分离，线程在后台运行
    this_thread::sleep_for(chrono::seconds(1));  // 主线程等待
    return 0;
}
```
- `detach()` 使得线程在后台运行，主线程无需等待其完成。

**注意**：如果一个线程被分离，主线程结束时系统会自动清理这个线程，但在多线程程序中，确保线程的状态管理是非常重要的，避免出现资源泄漏等问题。

---

### **6. 线程安全的数据结构**

C++ 标准库提供了多种线程安全的数据结构（如 `mutex`, `atomic`）来保证并发访问数据时的安全性。对于更高级的线程同步，可以使用 `atomic` 实现无锁编程。

#### **`atomic`**

`atomic` 提供了对基本数据类型的原子操作，使得数据在多线程环境下能够安全访问，而不需要使用显式的互斥锁。

```cpp
#include <iostream>
#include <atomic>
#include <thread>

atomic<int> counter(0);

void increment() {
    for (int i = 0; i < 1000; ++i) {
        ++counter;
    }
}

int main() {
    thread t1(increment);
    thread t2(increment);
    
    t1.join();
    t2.join();

    cout << "Counter: " << counter << endl;  // 线程安全的计数器
    return 0;
}
```
- `atomic<int> counter(0)`：定义一个原子计数器，避免竞争条件。
- `++counter`：自动进行原子操作，确保线程安全。

---

### **总结**

- **线程创建与管理**：使用 `thread` 创建线程，并使用 `join()` 等方法管理线程的生命周期。
- **线程同步**：通过 `mutex` 和 `condition_variable` 等同步工具，确保多个线程安全地共享资源。
- **线程通信**：使用条件变量和队列等机制，实现线程间的数据交换与协调。
- **线程生命周期管理**：通过 `join()` 和 `detach()` 等方法管理线程的生命周期。

C++ 提供了强大的线程支持，使得多线程编程变得更加简单和高效，特别适合在嵌入式系统中实现并行计算和多任务处理。在嵌入式开发中，合理设计线程模型和同步机制至关重要，以确保系统的实时性和稳定性。