import sys
import matplotlib.pyplot as plt

import matplotlib as mpl
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math
import os

from matplotlib.pyplot import figure

from math import atan2, degrees
import numpy as np

def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'
    #mpl.rc('font', family=fontName)
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
    #mpl.rc('text', color=textColor)
    mpl.rc('text', hinting='native')
    siz = 22.5  # 7.97
    pgf_with_latex = {
        "pgf.texsystem": "pdflatex",
        "text.usetex": True,
        "font.family": "serif",
        "font.serif": [],
        "font.sans-serif": [],
        "font.monospace": [],
        "axes.titlesize":  siz,
        "axes.labelsize":  siz,
        "font.size":  siz,
        "legend.fontsize":  siz,
        "xtick.labelsize":  siz,
        "ytick.labelsize":  siz,
        # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
        "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)

os.system("mkdir ../../results/")
os.system("mkdir ../../results/figure_8/")
os.system("mkdir ../../results/figure_8/data")

base_path = "../../results/figure_9/data/"
path_to_results = "../../results/figure_9/"

ext_xs = []
ext_vals = []
maj_xs = []
maj_vals = []

maj = 1.5

ext_data = open(base_path + "const.txt").readlines()

for j in range(0, int(len(ext_data) / 2)):
    ext_xs.append(float(ext_data[2*j]))
    ext_vals.append(float(ext_data[2*j+1]))
    maj_xs.append(float(ext_data[2*j]))
    maj_vals.append(maj)

setup()

figure(figsize=(6, 3), dpi=80)

plt.xlabel("x")
plt.ylabel("Extinction")

plt.plot(ext_xs, ext_vals, linewidth=5.0, label="$\\mu_r(x)$")
plt.plot(maj_xs, maj_vals, linewidth=5.0, label="$\\mu_t$")

index = 0
fill_xs = []
fill_y1 = []
fill_y2 = []

while index < len(ext_xs) and ext_xs[index] < 1.0:
    fill_xs.append(ext_xs[index])
    fill_y1.append(ext_vals[index])
    fill_y2.append(maj)
    index = index + 1

print(len(fill_y1))
print(len(fill_y2))

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")

plt.xlim(0.0, 1.0)
plt.legend()
# plt.tight_layout(rect=[-0.04,-0.09,1.045,1.085])
plt.tight_layout(rect=[-0.055,-0.13,1.072,1.11])
plt.savefig(path_to_results + "gr_const.png")
plt.savefig(path_to_results + "gr_const.pdf")
plt.savefig(path_to_results + "gr_const.pgf")
plt.clf()


####################################################

ext_xs = []
ext_vals = []
maj_xs = []
maj_vals = []

maj = 3.0

ext_data = open(base_path + "lin.txt").readlines()

for j in range(0, int(len(ext_data) / 2)):
    ext_xs.append(float(ext_data[2*j]))
    ext_vals.append(float(ext_data[2*j+1]))
    maj_xs.append(float(ext_data[2*j]))
    maj_vals.append(maj)

setup()

figure(figsize=(6, 3), dpi=80)

plt.xlabel("x")
plt.ylabel("Extinction")

plt.plot(ext_xs, ext_vals, linewidth=5.0, label="$\\mu_r(x)$")
plt.plot(maj_xs, maj_vals, linewidth=5.0, label="$\\mu_t$")

index = 0
fill_xs = []
fill_y1 = []
fill_y2 = []

while index < len(ext_xs) and ext_xs[index] < 1.0:
    fill_xs.append(ext_xs[index])
    fill_y1.append(ext_vals[index])
    fill_y2.append(maj)
    index = index + 1

print(len(fill_y1))
print(len(fill_y2))

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")

plt.xlim(0.0, 1.0)
plt.legend()
# plt.tight_layout(rect=[-0.04,-0.09,1.045,1.085])
plt.tight_layout(rect=[-0.055,-0.13,1.072,1.11])
plt.savefig(path_to_results + "gr_lin.png")
plt.savefig(path_to_results + "gr_lin.pdf")
plt.savefig(path_to_results + "gr_lin.pgf")
plt.clf()



####################################################

ext_xs = []
ext_vals = []
maj_xs = []
maj_vals = []

maj = 3.09

ext_data = open(base_path + "gauss.txt").readlines()

for j in range(0, int(len(ext_data) / 2)):
    ext_xs.append(float(ext_data[2*j]))
    ext_vals.append(float(ext_data[2*j+1]))
    maj_xs.append(float(ext_data[2*j]))
    maj_vals.append(maj)

setup()

figure(figsize=(6, 3), dpi=80)

plt.xlabel("x")
plt.ylabel("Extinction")

plt.plot(ext_xs, ext_vals, linewidth=5.0, label="$\\mu_r(x)$")
plt.plot(maj_xs, maj_vals, linewidth=5.0, label="$\\mu_t$")

index = 0
fill_xs = []
fill_y1 = []
fill_y2 = []

while index < len(ext_xs) and ext_xs[index] < 1.0:
    fill_xs.append(ext_xs[index])
    fill_y1.append(ext_vals[index])
    fill_y2.append(maj)
    index = index + 1

print(len(fill_y1))
print(len(fill_y2))

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")
# plt.axvline(x = 0.6, color = 'k', linewidth=5.0)

plt.xlim(0.0, 1.0)
plt.legend()
# plt.tight_layout(rect=[-0.04,-0.09,1.045,1.085])
plt.tight_layout(rect=[-0.055,-0.13,1.072,1.11])
plt.savefig(path_to_results + "gr_gauss.png")
plt.savefig(path_to_results + "gr_gauss.pdf")
plt.savefig(path_to_results + "gr_gauss.pgf")
plt.clf()


####################################################

ext_xs = []
ext_vals = []
maj_xs = []
maj_vals = []

maj = 2.47

ext_data = open(base_path + "comp_sine.txt").readlines()

for j in range(0, int(len(ext_data) / 2)):
    ext_xs.append(float(ext_data[2*j]))
    ext_vals.append(float(ext_data[2*j+1]))
    maj_xs.append(float(ext_data[2*j]))
    maj_vals.append(maj)

setup()

figure(figsize=(6, 3), dpi=80)

plt.xlabel("x")
plt.ylabel("Extinction")

plt.plot(ext_xs, ext_vals, linewidth=5.0, label="$\\mu_r(x)$")
plt.plot(maj_xs, maj_vals, linewidth=5.0, label="$\\mu_t$")

index = 0
fill_xs = []
fill_y1 = []
fill_y2 = []

while index < len(ext_xs) and ext_xs[index] < 1.0:
    fill_xs.append(ext_xs[index])
    fill_y1.append(ext_vals[index])
    fill_y2.append(maj)
    index = index + 1

print(len(fill_y1))
print(len(fill_y2))

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")

# plt.ylim(0.9, 1.7)
plt.xlim(0.0, 1.0)
plt.legend()
# plt.tight_layout(rect=[-0.01,-0.09,1.045,1.085])
plt.tight_layout(rect=[-0.055,-0.13,1.072,1.11])
plt.savefig(path_to_results + "gr_comp_sine.png")
plt.savefig(path_to_results + "gr_comp_sine.pdf")
plt.savefig(path_to_results + "gr_comp_sine.pgf")
plt.clf()

####################################################

figure(figsize=(6, 3), dpi=80)

setup()

plt.xlabel("spp")
plt.ylabel("Cost * Variance")

xs = []
rat_vals = []
ours_vals = []

rat_data = []
rat_data = open(base_path + "const_res.txt").readlines()

for j in range(0, int(len(rat_data) / 3)):
    xs.append(float(rat_data[3*j]))
    ours_vals.append(float(rat_data[3*j+1]))
    rat_vals.append(float(rat_data[3*j+2]))

plt.plot(xs, rat_vals, linewidth=5.0, label="ratio")
plt.plot(xs, ours_vals, linewidth=5.0, label="ours")

plt.legend()

plt.xlim(8, 65536)

plt.xscale('log', base=2)
plt.yscale('log', base=2)

# plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.tight_layout(rect=[-0.055,-0.11,1.055,1.11])
plt.savefig(path_to_results + "const_final.png")
plt.savefig(path_to_results + "const_final.pdf")
plt.savefig(path_to_results + "const_final.pgf")
plt.clf()

#############################################################

figure(figsize=(6, 3), dpi=80)

setup()

plt.xlabel("spp")
plt.ylabel("Cost * Variance")

xs = []
rat_vals = []
ours_vals = []

rat_data = []
rat_data = open(base_path + "lin_res.txt").readlines()

for j in range(0, int(len(rat_data) / 3)):
    xs.append(float(rat_data[3*j]))
    ours_vals.append(float(rat_data[3*j+1]))
    rat_vals.append(float(rat_data[3*j+2]))

plt.plot(xs, rat_vals, linewidth=5.0, label="ratio")
plt.plot(xs, ours_vals, linewidth=5.0, label="ours")

plt.legend()

plt.xlim(8, 65536)

plt.xscale('log', base=2)
plt.yscale('log', base=2)

# plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.tight_layout(rect=[-0.055,-0.11,1.055,1.11])
plt.savefig(path_to_results + "lin_final.png")
plt.savefig(path_to_results + "lin_final.pdf")
plt.savefig(path_to_results + "lin_final.pgf")
plt.clf()

#############################################################

figure(figsize=(6, 3), dpi=80)

setup()

plt.xlabel("spp")
plt.ylabel("Cost * Variance")

xs = []
rat_vals = []
ours_vals = []

rat_data = []
rat_data = open(base_path + "comp_sine_res.txt").readlines()

for j in range(0, int(len(rat_data) / 3)):
    xs.append(float(rat_data[3*j]))
    ours_vals.append(float(rat_data[3*j+1]))
    rat_vals.append(float(rat_data[3*j+2]))

plt.plot(xs, rat_vals, linewidth=5.0, label="ratio")
plt.plot(xs, ours_vals, linewidth=5.0, label="ours")

plt.legend()

plt.xlim(8, 65536)

plt.xscale('log', base=2)
plt.yscale('log', base=2)

# plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.tight_layout(rect=[-0.055,-0.11,1.055,1.11])
plt.savefig(path_to_results + "comp_sine_final.png")
plt.savefig(path_to_results + "comp_sine_final.pdf")
plt.savefig(path_to_results + "comp_sine_final.pgf")
plt.clf()


#############################################################

figure(figsize=(6, 3), dpi=80)

setup()

plt.xlabel("spp")
plt.ylabel("Cost * Variance")

xs = []
rat_vals = []
ours_vals = []

rat_data = []
rat_data = open(base_path + "gauss_res.txt").readlines()

for j in range(0, int(len(rat_data) / 3)):
    xs.append(float(rat_data[3*j]))
    ours_vals.append(float(rat_data[3*j+1]))
    rat_vals.append(float(rat_data[3*j+2]))

plt.plot(xs, rat_vals, linewidth=5.0, label="ratio")
plt.plot(xs, ours_vals, linewidth=5.0, label="ours")

plt.legend()

plt.xlim(8, 65536)

plt.xscale('log', base=2)
plt.yscale('log', base=2)

# plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.tight_layout(rect=[-0.055,-0.11,1.055,1.11])
plt.savefig(path_to_results + "gauss_final.png")
plt.savefig(path_to_results + "gauss_final.pdf")
plt.savefig(path_to_results + "gauss_final.pgf")
plt.clf()
