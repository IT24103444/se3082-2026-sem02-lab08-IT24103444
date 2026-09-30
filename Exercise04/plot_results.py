import csv
import matplotlib.pyplot as plt

processes = []
times = []
speedups = []

with open("timing_data.csv", "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        processes.append(int(row["Processes"]))
        times.append(float(row["ExecutionTime"]))
        speedups.append(float(row["Speedup"]))

# Graph 1: Time vs Number of Processors
plt.figure()
plt.plot(processes, times, marker="o")
plt.xlabel("Number of Processors")
plt.ylabel("Execution Time (seconds)")
plt.title("Execution Time vs Number of Processors")
plt.xticks(processes)
plt.grid(True)
plt.tight_layout()
plt.savefig("time_vs_processors.png")
plt.close()

# Graph 2: Speedup
plt.figure()
plt.plot(processes, speedups, marker="o")
plt.xlabel("Number of Processors")
plt.ylabel("Speedup")
plt.title("Speedup vs Number of Processors")
plt.xticks(processes)
plt.grid(True)
plt.tight_layout()
plt.savefig("speedup.png")
plt.close()

print("Graphs generated successfully.")
