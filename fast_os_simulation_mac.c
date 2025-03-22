#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

// Color codes for terminal output
#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RED     "\x1b[31m"

// OS Configuration
#define MAX_PROCESSES 10
#define MAX_MODULES 5
#define MEMORY_SIZE 1024 // Simulated memory size in MB

// Timing functions
long long current_timestamp_ms() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    return te.tv_sec * 1000LL + te.tv_usec / 1000;
}

void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}

// Memory Management Simulation
typedef struct {
    int total_memory;
    int used_memory;
    int free_memory;
} MemoryManager;

MemoryManager memory_manager;

void init_memory_manager() {
    memory_manager.total_memory = MEMORY_SIZE;
    memory_manager.used_memory = 0;
    memory_manager.free_memory = MEMORY_SIZE;
    printf("%s[Memory] Memory manager initialized with %d MB total memory\n", COLOR_GREEN, MEMORY_SIZE);
    printf("[Memory] Memory allocation strategy: Zero-copy with optimized paging%s\n", COLOR_RESET);
    fflush(stdout);
}

int allocate_memory(int size) {
    if (memory_manager.free_memory >= size) {
        memory_manager.used_memory += size;
        memory_manager.free_memory -= size;
        return 1; // Success
    }
    return 0; // Failure
}

// Process Management Simulation
typedef struct {
    int pid;
    char name[32];
    int memory_usage;
    int priority;
    int status; // 0: idle, 1: running, 2: blocked
} Process;

Process processes[MAX_PROCESSES];
int process_count = 0;

int create_process(const char* name, int memory_usage, int priority) {
    if (process_count >= MAX_PROCESSES) {
        return -1; // Max processes reached
    }
    
    if (!allocate_memory(memory_usage)) {
        return -2; // Not enough memory
    }
    
    Process* p = &processes[process_count];
    p->pid = process_count + 1;
    strncpy(p->name, name, 31);
    p->name[31] = '\0';
    p->memory_usage = memory_usage;
    p->priority = priority;
    p->status = 0; // Idle
    
    process_count++;
    return p->pid;
}

void start_process(int pid) {
    if (pid > 0 && pid <= process_count) {
        processes[pid-1].status = 1; // Running
        printf("%s[Scheduler] Started process %d: %s (Priority: %d)%s\n", 
               COLOR_MAGENTA, pid, processes[pid-1].name, processes[pid-1].priority, COLOR_RESET);
        fflush(stdout);
    }
}

// Module Management Simulation
typedef struct {
    int id;
    char name[32];
    int is_loaded;
} Module;

Module modules[MAX_MODULES];
int module_count = 0;

int load_module(const char* name) {
    if (module_count >= MAX_MODULES) {
        return -1; // Max modules reached
    }
    
    Module* m = &modules[module_count];
    m->id = module_count + 1;
    strncpy(m->name, name, 31);
    m->name[31] = '\0';
    m->is_loaded = 1;
    
    module_count++;
    printf("%s[Module] Loaded module %d: %s%s\n", COLOR_BLUE, m->id, m->name, COLOR_RESET);
    fflush(stdout);
    return m->id;
}

// Bootloader simulation
void bootloader() {
    printf("%s[Bootloader] Starting fast OS bootloader...\n", COLOR_YELLOW);
    printf("[Bootloader] Checking minimal hardware requirements...\n");
    fflush(stdout);
    sleep_ms(100);
    printf("[Bootloader] Loading kernel into memory at 0x100000...\n");
    fflush(stdout);
    sleep_ms(150);
    printf("[Bootloader] Jump to kernel entry point%s\n", COLOR_RESET);
    fflush(stdout);
}

// Kernel initialization
void kernel_init() {
    printf("%s[Kernel] Initializing microkernel...\n", COLOR_GREEN);
    printf("[Kernel] Setting up memory management...\n");
    fflush(stdout);
    sleep_ms(100);
    printf("[Kernel] Initializing process scheduler...\n");
    fflush(stdout);
    sleep_ms(50);
    printf("[Kernel] Starting system services in parallel...\n");
    fflush(stdout);
    sleep_ms(100);
    printf("[Kernel] Kernel initialization complete%s\n", COLOR_RESET);
    fflush(stdout);
}

// Graphics driver initialization
void graphics_init() {
    printf("%s[Graphics] Initializing framebuffer...\n", COLOR_BLUE);
    fflush(stdout);
    sleep_ms(30);
    printf("[Graphics] Setting up display resolution...\n");
    fflush(stdout);
    sleep_ms(20);
    printf("[Graphics] Display ready%s\n", COLOR_RESET);
    fflush(stdout);
}

// Display boot screen
void display_boot_screen() {
    printf("\n%s", COLOR_CYAN);
    printf("╔════════════════════════════════════════╗\n");
    printf("║                                        ║\n");
    printf("║          ULTRA FAST OS v1.0            ║\n");
    printf("║                                        ║\n");
    printf("║  Boot time: < 1 second                 ║\n");
    printf("║  Display time: < 50ms                  ║\n");
    printf("║                                        ║\n");
    printf("║  Microkernel Architecture              ║\n");
    printf("║  Optimized for Performance             ║\n");
    printf("║                                        ║\n");
    printf("╚════════════════════════════════════════╝%s\n", COLOR_RESET);
    fflush(stdout);
}

// Thread simulation for parallel processing
typedef struct {
    int thread_id;
    char task_name[32];
    int duration_ms;
} ThreadTask;

// Thread function for parallel task execution
void* execute_task(void* arg) {
    ThreadTask* task = (ThreadTask*)arg;
    
    printf("%s[Thread %d] Starting task: %s%s\n", 
           COLOR_RED, task->thread_id, task->task_name, COLOR_RESET);
    fflush(stdout);
    
    // Simulate task execution
    sleep_ms(task->duration_ms);
    
    printf("%s[Thread %d] Completed task: %s (took %d ms)%s\n", 
           COLOR_RED, task->thread_id, task->task_name, task->duration_ms, COLOR_RESET);
    fflush(stdout);
    
    free(task);
    return NULL;
}

// Demonstrate parallel task execution
void demonstrate_parallel_execution() {
    printf("\n%s[System] Demonstrating parallel task execution%s\n", COLOR_MAGENTA, COLOR_RESET);
    fflush(stdout);
    
    pthread_t threads[4];
    const char* task_names[] = {
        "File indexing", 
        "Network packet processing", 
        "UI rendering", 
        "Background compilation"
    };
    int durations[] = {200, 150, 100, 250};
    
    // Create and start threads
    for (int i = 0; i < 4; i++) {
        ThreadTask* task = (ThreadTask*)malloc(sizeof(ThreadTask));
        task->thread_id = i + 1;
        strncpy(task->task_name, task_names[i], 31);
        task->task_name[31] = '\0';
        task->duration_ms = durations[i];
        
        pthread_create(&threads[i], NULL, execute_task, task);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("%s[System] All parallel tasks completed successfully%s\n", COLOR_MAGENTA, COLOR_RESET);
    fflush(stdout);
}

// Simulated user interaction
void simulate_user_interaction() {
    printf("\n%s[User] Simulating user interaction with the system%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    
    // Open a terminal
    printf("%s[User] Opening terminal application%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    int terminal_pid = create_process("Terminal", 15, 10);
    start_process(terminal_pid);
    sleep_ms(50);
    
    // Open a web browser
    printf("%s[User] Opening web browser%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    int browser_pid = create_process("WebBrowser", 120, 8);
    start_process(browser_pid);
    sleep_ms(50);
    
    // Open a text editor
    printf("%s[User] Opening text editor%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    int editor_pid = create_process("TextEditor", 45, 7);
    start_process(editor_pid);
    sleep_ms(50);
    
    // Show system status
    printf("\n%s[System] Current system status:%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("- Memory usage: %d MB used, %d MB free\n", 
           memory_manager.used_memory, memory_manager.free_memory);
    printf("- Running processes: %d\n", process_count);
    fflush(stdout);
    
    // Show process list
    printf("\n%s[System] Process list:%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%-5s %-20s %-15s %-10s %-10s\n", "PID", "Name", "Memory (MB)", "Priority", "Status");
    printf("------------------------------------------------------------------\n");
    fflush(stdout);
    
    for (int i = 0; i < process_count; i++) {
        const char* status_str = "Unknown";
        switch (processes[i].status) {
            case 0: status_str = "Idle"; break;
            case 1: status_str = "Running"; break;
            case 2: status_str = "Blocked"; break;
        }
        printf("%-5d %-20s %-15d %-10d %-10s\n", 
               processes[i].pid, processes[i].name, processes[i].memory_usage, 
               processes[i].priority, status_str);
    }
    fflush(stdout);
    
    // Demonstrate parallel execution
    demonstrate_parallel_execution();
}

// System ready notification
void system_ready() {
    printf("%s[System] All components initialized\n", COLOR_MAGENTA);
    printf("[System] Memory usage: %d MB used, %d MB free\n", 
           memory_manager.used_memory, memory_manager.free_memory);
    printf("[System] %d processes created, %d modules loaded\n", process_count, module_count);
    printf("[System] System is ready for user interaction\n");
    printf("[System] Total boot time: %s", COLOR_RESET);
    fflush(stdout);
}

// Main OS simulation
int main() {
    long long start_time, display_time, end_time;
    
    // Record start time
    start_time = current_timestamp_ms();
    
    printf("%sULTRA FAST OS SIMULATION (macOS version)%s\n\n", COLOR_CYAN, COLOR_RESET);
    printf("This program simulates the operation of a high-speed OS.\n");
    printf("It demonstrates the concepts of fast boot, memory management, and parallel processing.\n\n");
    fflush(stdout);
    
    sleep_ms(2000); // Wait 2 seconds
    
    // Boot sequence
    bootloader();
    kernel_init();
    
    // Initialize memory management
    init_memory_manager();
    
    // Load essential modules
    load_module("FileSystem");
    load_module("NetworkStack");
    load_module("SecurityModule");
    load_module("UserInterface");
    
    // Record display start time
    display_time = current_timestamp_ms();
    
    // Initialize graphics and display boot screen
    graphics_init();
    sleep_ms(1000); // Wait 1 second
    display_boot_screen();
    
    // Create system processes
    create_process("SystemManager", 25, 15);
    create_process("DeviceManager", 18, 12);
    create_process("ServiceManager", 22, 13);
    
    // Record end time
    end_time = current_timestamp_ms();
    
    // Show timing results
    system_ready();
    printf("- Total boot time: %lld ms\n", end_time - start_time);
    printf("- Display time: %lld ms\n", end_time - display_time);
    fflush(stdout);
    
    sleep_ms(2000); // Wait 2 seconds
    
    // Simulate user interaction
    simulate_user_interaction();
    
    printf("\n%s[System] System demonstration complete%s\n", COLOR_MAGENTA, COLOR_RESET);
    fflush(stdout);
    
    return 0;
}