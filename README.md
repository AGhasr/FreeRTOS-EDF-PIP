# FreeRTOS Kernel Modification: Earliest Deadline First (EDF) Scheduling & Priority Inheritance Protocol (PIP)

Modification of the FreeRTOS kernel for the ESP32 (ESP-IDF framework). This project replaces the native static fixed-priority scheduler with a dynamic **Earliest Deadline First (EDF)** scheduling algorithm and integrates a **Deadline Inheritance Protocol (PIP)** to prevent deadline inversion during resource contention.

Developed as part of my Bachelor's Thesis: *"Earliest Deadline First Scheduling and Deadline Inheritance in FreeRTOS"*.

---

##  Key Features & Kernel Architecture

### 1. Dynamic EDF Scheduler
* **Ready List Restructuring:** Replaced the native `pxReadyTasksLists` array (one list per priority) with a single, unified `pxReadyTasksList` sorted dynamically by absolute deadlines ($xCurrentDeadline$).
* **O(1) Task Selection:** Optimized the scheduler context switch using a custom macro `taskSELECT_EARLIEST_DEADLINE_TASK()`, which instantly grabs the head entry of the sorted ready list.
* **Deadline-Based Preemption:** Overrode FreeRTOS preemption checks (`taskIS_YIELD_REQUIRED`) to evaluate execution urgency via dynamic deadline comparisons instead of static priority matching.

### 2. Deadline Inheritance Protocol (PIP)
* Prevents real-time scheduling constraints from failing due to priority/deadline inversion.
* When a high-urgency task blocks on a shared resource (e.g., a Mutex), the resource holder automatically inherits the earlier deadline (`xTaskDeadlineInherit`).
* **Nested Mutex Handling:** Accurately calculates and restores intermediate deadlines during complex, nested mutex acquisition states upon resource release (`xTaskDeadlineDisinherit`).

### 3. TCB (Task Control Block) Extensions
Extended the core FreeRTOS `tskTCB` structure with tracking metrics:
* `xBaseDeadline`: Constant relative deadline interval.
* `xCurrentDeadline`: Dynamically inherited/disinherited deadline during resource lockups.
* `xAbsoluteDeadline`: Re-evaluated at every new task activation loop within the tick handler (`xTaskIncrementTick`).
* `waitingTasks`: A custom dependency-tracking list attached to each task to efficiently trace nested blocking chains.

---

## Repository Structure

The modified kernel is isolated inside the local `components/` block to bypass submodule dependencies:

```text
├── components/
│   └── freertos/        # Modified FreeRTOS Kernel source code
│       ├── tasks.c      # Modified TCB definitions, scheduler loop, PIP routines
│       ├── queue.c      # Event list modifications mapped to EDF metrics
│       └── ...
├── main/
│   ├── main.c           # Script validating L, M, H task behavior
│   └── CMakeLists.txt
├── CMakeLists.txt
└── README.md

```

## Simple Demo (`main.c`)

The testbench application spins up three tasks modeling classic real-time workloads with distinct urgency tiers and constraints:

* **Task L (Low Urgency):** Base Deadline = 1000ms | Execution Budget = 500ms
* **Task M (Medium Urgency):** Base Deadline = 800ms | Execution Budget = 300ms
* **Task H (High Urgency):** Base Deadline = 600ms | Execution Budget = 100ms


The application demonstrates:

- EDF task ordering
- Dynamic preemption
- Deadline inheritance during mutex contention
- Correct restoration after mutex release

---

## Building

This project uses the **ESP-IDF** build system.

### Build

```bash
idf.py build
```

### Flash and Monitor

```bash
idf.py -p <PORT> flash monitor
```
