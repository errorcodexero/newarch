#!/usr/bin/env python
import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description="Plot motion profile from robot log")
parser.add_argument("log_path", help="path to the robot log file")

log_file = open(parser.parse_args().log_path, "r")
log = log_file.read()
log_file.close()

separator = "********** Robot program starting **********\n"
log = log[log.find(separator) + len(separator):]
lines = log.split("\n")

data = []
for line in lines:
	if (len(line) > 0):
        	halves = line.split(" / ")
		time = float(halves[0])
		distances = [float(n) for n in halves[1].split(":")]
		outputs = [float(n) for n in halves[2].split(":")]
		current_angle = float(halves[3])
		goal_angle = float(halves[4])
		error_d = float(halves[5])
		error_i = float(halves[6])
		target_dist = float(halves[7])
        	data.append({"t": time, "dist_l": distances[0], "dist_r": distances[1], "out_l": outputs[0], "out_r": outputs[1], "current_angle": current_angle, "goal_angle": goal_angle, "error_d": error_d, "error_i": error_i, "target_dist": target_dist})

plt.figure(1)

plt.subplot(221)
plt.plot([d["t"] for d in data], [d["out_l"] for d in data])
plt.plot([d["t"] for d in data], [d["out_r"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("output")

plt.subplot(222)
plt.plot([d["t"] for d in data], [d["dist_l"] for d in data])
plt.plot([d["t"] for d in data], [d["dist_r"] for d in data])
plt.plot([d["t"] for d in data], [d["target_dist"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("distance (in)")

plt.subplot(223)
plt.plot([d["t"] for d in data], [d["goal_angle"] for d in data])
plt.plot([d["t"] for d in data], [d["current_angle"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("angle (degrees)")

plt.subplot(224)
plt.plot([d["t"] for d in data], [d["dist_l"] - d["dist_r"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("distance difference (in)")

plt.subplots_adjust(wspace=.5, hspace=.5)
plt.suptitle("Log Values")

plt.figure(2)

plt.subplot(211)
plt.plot([d["t"] for d in data], [d["error_d"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("Error Derivative")

plt.subplot(212)
plt.plot([d["t"] for d in data], [d["error_i"] for d in data])
plt.xlabel("time (s)")
plt.ylabel("Error Integral")

plt.subplots_adjust(wspace=.5, hspace=.5)
plt.suptitle("Log Values")

plt.show()
