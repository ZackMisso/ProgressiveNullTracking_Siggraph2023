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
import math

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

figure(figsize=(6, 3), dpi=80)

setup()

os.system("mkdir ../../results/")
os.system("mkdir ../../results/figure_7/")

path_to_results = "../../results/figure_7/"

#################### IMPORTANCE SAMPLE GRAPH ####################

null_dense = 1.0
maj_dense = 3.0

dists = []
null = []
maj = []

min_dist = 0.01
max_dist = 5.0
step = 0.01

plt.xlabel("t")
plt.ylabel("$p_{MC}(t)$")

dist = min_dist
while (dist <= max_dist):
    dists.append(dist)
    maj.append(maj_dense * math.exp(-maj_dense * dist))
    null.append(null_dense * math.exp(-null_dense * dist))
    dist = dist + step

plt.plot(dists, maj, linewidth=5.0, label="$p_{MC}(t) = \\mu_t \\bar{Tr}(0,t)$")
plt.plot(dists, null, linewidth=5.0, label="$p_{MC}(t) = \\mu_n \\hat{Tr}(0,t)$")

# plt.ylim(0.0, 1.0)
plt.xlim(0.0, 3.0)
plt.legend()
# plt.tight_layout(rect=[-0.04,-0.09,1.04,1.08])
plt.tight_layout(rect=[-0.055,-0.13,1.055,1.11])

plt.savefig(path_to_results + "imp_samp.png")
plt.savefig(path_to_results + "imp_samp.pdf")
plt.savefig(path_to_results + "imp_samp.pgf")

plt.clf()

#################### GAUSSIAN EXTINCTION ####################

figure(figsize=(6, 3), dpi=80)

setup()

base_path = "../results/exp_adaptive_comp/test/"

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

y1 = np.array(fill_y1)
y2 = np.array(fill_y2)

matplotlib.pyplot.fill_between(fill_xs, y1, y2, where=(y1 < y2), interpolate=True, alpha = 0.2, color="blue")
plt.axvline(x = 0.6, color = 'k', linewidth=5.0)

# plt.ylim(0.9, 1.7)
plt.xlim(0.0, 1.0)
plt.legend()
# plt.tight_layout(rect=[-0.04,-0.09,1.045,1.085])
plt.tight_layout(rect=[-0.055,-0.13,1.074,1.11])
plt.savefig(path_to_results + "gr_gauss.png")
plt.savefig(path_to_results + "gr_gauss.pdf")
plt.savefig(path_to_results + "gr_gauss.pgf")
plt.clf()
