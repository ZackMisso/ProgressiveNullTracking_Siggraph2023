# import setup
import sys
import matplotlib.pyplot as plt
# import setup

import matplotlib as mpl
# import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math
# from labellines import labelLines

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

figure(figsize=(8, 4), dpi=80)

setup()

trials = 100
base_path = "/Users/corneria/Documents/Research/bias_research/progressive_controls/6398d9dc531c9eefb55fa6a3/figures/bunny/"
path_to_results = base_path

plt.xlabel("spp")
plt.ylabel("MSE")

xs = []
rat_vals = []
ours_vals = []

rat_data = []
rat_data = open(base_path + "mses.txt").readlines()

for j in range(0, int(len(rat_data) / 3)):
    xs.append(float(rat_data[3*j]))
    rat_vals.append(float(rat_data[3*j+1]))
    ours_vals.append(float(rat_data[3*j+2]))

plt.plot(xs, rat_vals, linewidth=5.0, label="ratio")
plt.plot(xs, ours_vals, linewidth=5.0, label="ours")

# plt.plot(rat_xs, rat_vals, "-b", alpha=0.03)

plt.legend()

plt.xlim(8, 1024)

plt.xscale('log', base=2)
plt.yscale('log', base=2)

plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.savefig(path_to_results + "convergence.png")
plt.savefig(path_to_results + "convergence.pdf")
plt.savefig(path_to_results + "convergence.pgf")
plt.clf()

