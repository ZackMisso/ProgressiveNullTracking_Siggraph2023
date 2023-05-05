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

base_path = "../results/exp_adaptive_comp/test/"
path_to_results = "../results/exp_adaptive_comp/"

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

plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.savefig(path_to_results + "const_final.png")
plt.savefig(path_to_results + "const_final.pdf")
plt.savefig(path_to_results + "const_final.pgf")
plt.clf()

#############################################################

figure(figsize=(8, 4), dpi=80)

setup()

base_path = "../results/exp_adaptive_comp/test/"
path_to_results = "../results/exp_adaptive_comp/"

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

plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.savefig(path_to_results + "lin_final.png")
plt.savefig(path_to_results + "lin_final.pdf")
plt.savefig(path_to_results + "lin_final.pgf")
plt.clf()

#############################################################

figure(figsize=(8, 4), dpi=80)

setup()

base_path = "../results/exp_adaptive_comp/test/"
path_to_results = "../results/exp_adaptive_comp/"

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

plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.savefig(path_to_results + "comp_sine_final.png")
plt.savefig(path_to_results + "comp_sine_final.pdf")
plt.savefig(path_to_results + "comp_sine_final.pgf")
plt.clf()


#############################################################

figure(figsize=(8, 4), dpi=80)

setup()

base_path = "../results/exp_adaptive_comp/test/"
path_to_results = "../results/fuck_me/"

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

plt.tight_layout(rect=[-0.04,-0.08,1.04,1.08])
plt.savefig(path_to_results + "gauss_final.png")
plt.savefig(path_to_results + "gauss_final.pdf")
plt.savefig(path_to_results + "gauss_final.pgf")
plt.clf()
