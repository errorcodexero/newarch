#!/usr/bin/env python
import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description="Plot motion profile from drivebase test log")
parser.add_argument("log_path", help="path to the drivebase log file")

log_file = open(parser.parse_args().log_path, "r")
log = log_file.read()
log_file.close()

separator = "==========================================================\n"
log_end = "Finishing"
log = log[log.find(separator) + len(separator):log.find(log_end) - 1]
lines = log.split("\n")

data = []
distance_start = "Distances("
output_start = "Drivebase::Output( "
for line in lines:
	time = float(line[2:line.find("\t")])
	distance_pos = line.rfind(distance_start) + len(distance_start)
	distances_str = line[distance_pos:line.find(" )", distance_pos)]
	distances = [float(n) for n in distances_str.split(" ")]
	output_pos = line.find(output_start) + len(output_start)
	outputs_str = line[output_pos:line.find(" )", output_pos)]
	outputs = [float(n) for n in outputs_str.split(" ")]
	data.append({"t": time, "dist_l": distances[0], "dist_r": distances[1], "out_l": outputs[0], "out_r": outputs[1]})

plt.figure(1)

plt.subplot(221)
plt.plot([d["t"] for d in data], [d["out_l"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("left output")

plt.subplot(222)
plt.plot([d["t"] for d in data], [d["out_r"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("right output")

plt.subplot(223)
plt.plot([d["t"] for d in data], [d["dist_l"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("left distance (in)")

plt.subplot(224)
plt.plot([d["t"] for d in data], [d["dist_r"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("right distance (in)")

plt.subplots_adjust(wspace=.5, hspace=.5)
plt.suptitle("Motion Profiling")

plt.figure(2)

plt.subplot(121)
plt.plot([d["dist_l"] for d in data], [d["out_l"] for d in data])
plt.xlabel("left distance (in)")
plt.ylabel("left output")

plt.subplot(122)
plt.plot([d["dist_r"] for d in data], [d["out_r"] for d in data])
plt.xlabel("right distance (in)")
plt.ylabel("right output")

plt.subplots_adjust(wspace=.5)
plt.suptitle("Motion Profiling")

plt.show()
