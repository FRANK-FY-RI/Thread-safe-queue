# Thread-Safe Queue

A lock-based thread-safe queue implementation using `std::mutex` and `std::condition_variable`.

## Operations

### `try_pop`

A non-blocking pop operation. Returns immediately if the queue is empty.

Available in two forms:

* Returns the popped value through a reference parameter
* Returns the popped value as a `std::shared_ptr`

### `wait_and_pop`

A blocking pop operation. Waits until an element becomes available.

Available in two forms:

* Returns the popped value through a reference parameter
* Returns the popped value as a `std::shared_ptr`

## Project Structure

```text
.
├── threadsafe_queue.hpp
├── README.md
├── .gitignore
└── tests
    ├── push_test.cpp
    ├── push_and_pop_test.cpp
    └── Runtime_monitor.hpp
```

## Requirements

* C++11 or later

