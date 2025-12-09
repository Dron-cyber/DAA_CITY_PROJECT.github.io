#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

struct Task {
    std::string id;
    int urgency_score;
};

struct CompareTask {
    bool operator()(const Task& a, const Task& b) {
        return a.urgency_score > b.urgency_score;
    }
};

class TaskScheduler {
private:
    std::priority_queue<Task, std::vector<Task>, CompareTask> task_queue;

public:
    void add_task(const Task& t) {
        task_queue.push(t);
        std::cout << "-> ADDED: Task " << t.id << " (Score: " << t.urgency_score << ")" << std::endl;
    }

    Task get_next_task() {
        if (task_queue.empty()) {
            throw std::runtime_error("No tasks remaining for assignment.");
        }
        Task next_task = task_queue.top();
        task_queue.pop();
        std::cout << "\n<< ASSIGNED: Task " << next_task.id
                  << " (Priority Score: " << next_task.urgency_score << ") >>" << std::endl;
        return next_task;
    }

    bool is_empty() const {
        return task_queue.empty();
    }

    size_t size() const {
        return task_queue.size();
    }
};

int main() {
    TaskScheduler scheduler;
    std::string line;
    int num_tasks;

    std::cout << "--- Dynamic Resource Allocation System ---" << std::endl;
    std::cout << "Enter the total number of tasks to submit: ";
    if (!(std::cin >> num_tasks) || num_tasks <= 0) {
        std::cerr << "Invalid number of tasks. Exiting." << std::endl;
        return 1;
    }

    std::getline(std::cin, line);

    std::cout << "\nEnter tasks as: [TaskID] [UrgencyScore]" << std::endl;

    for (int i = 0; i < num_tasks; ++i) {
        std::string id;
        int score;
        std::cout << "Task " << i + 1 << "/" << num_tasks << ": ";
        if (!(std::cin >> id >> score) || score < 0) {
            std::cerr << "Invalid task format. Skipping." << std::endl;
            i--;
            continue;
        }
        scheduler.add_task({id, score});
    }

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "Processing Tasks in Order of Urgency..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    while (!scheduler.is_empty()) {
        try {
            scheduler.get_next_task();
            std::cout << "   Remaining tasks in queue: " << scheduler.size() << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            break;
        }
    }

    std::cout << "\n--- All tasks assigned. System ready for new input. ---" << std::endl;
    return 0;
}
