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

# Pre-revisions, this figure  existed in our paper and was used 
# to show the analytical variance for ratio tracking as a function
# of the extinction.
#
# We include it here in case anyone finds it interesting.

def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
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
        "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)

figure(figsize=(12, 4), dpi=80)

setup()

os.system("mkdir ../../results/")
os.system("mkdir ../../results/former_figure_4/")

path_to_results = "../../results/former_figure_4/"

real_dense = 1
bound_dense = 2
slight_dense = 0.7
signif_dense = 0.2

min_dist = 0.0
max_dist = 10
step = 0.01

dists = []
bound = []
tight = []
slight = []
signif = []

plt.xlabel("d")
plt.ylabel("Variance")

dist = min_dist
while (dist <= max_dist):
    dists.append(dist)
    real_term = math.exp(-2.0 * real_dense * dist)
    bound.append(math.exp((real_dense/bound_dense - 2.0) * real_dense * dist) - real_term)
    slight.append(math.exp((real_dense/slight_dense - 2.0) * real_dense * dist) - real_term)
    signif.append(math.exp((real_dense/signif_dense - 2.0) * real_dense * dist) - real_term)
    tight.append(math.exp((real_dense/real_dense - 2.0) * real_dense * dist) - real_term)
    dist = dist + step

plt.plot(dists, bound, linewidth=10.0, label="$\\mu_t = 2.0\\mu_r$; $\\alpha_n > 0$ ")
plt.plot(dists, tight, linewidth=10.0, label="$\\mu_t = 1.0\\mu_r$; $\\alpha_n = 0$ ")
plt.plot(dists, slight, linewidth=10.0, label="$\\mu_t = 0.7\\mu_r$; $-1 < \\alpha_n < 0$ ")
plt.plot(dists, signif, linewidth=10.0, label="$\\mu_t = 0.2\\mu_r$; $\\alpha_n < -1$ ")

plt.ylim(0.0, 1.0)
plt.xlim(0.0, 6.0)
plt.legend()
plt.tight_layout(rect=[-0.025,-0.09,1.028,1.08])

plt.savefig(path_to_results + "rat_var.png")
plt.savefig(path_to_results + "rat_var.pdf")
plt.savefig(path_to_results + "rat_var.pgf")

