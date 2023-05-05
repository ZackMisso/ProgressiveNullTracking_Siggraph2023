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
    mpl.rc('font', size=10)
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

path_to_results = "../../results/figure_3/"

plt.xlabel("x")
plt.ylabel("Extinction")

ext_xs = []
ext_vals = []

ext_xs.append(0.0)
ext_xs.append(5.0)
ext_vals.append(1.0)
ext_vals.append(1.0)

bound_xs = []
bound_vals = []

bound_xs.append(0.0)
bound_xs.append(5.0)
bound_vals.append(1.3)
bound_vals.append(1.3)

good_xs = []
good_vals = []

good_xs.append(0.0)
good_xs.append(5.0)
good_vals.append(0.7)
good_vals.append(0.7)

bad_xs = []
bad_vals = []

bad_xs.append(0.0)
bad_xs.append(5.0)
bad_vals.append(0.3)
bad_vals.append(0.3)


plt.plot(ext_xs, ext_vals, "-", linewidth=5.0, label="$\\mu_r$")
plt.plot(bound_xs, bound_vals, "-", linewidth=5.0,label="$\\mu_t = 1.3\\mu_r$")
plt.plot(good_xs, good_vals, "-", linewidth=5.0,label="$\\mu_t = 0.7\\mu_r$")
plt.plot(bad_xs, bad_vals, "-", linewidth=5.0,label="$\\mu_t = 0.3\\mu_r$")

plt.legend()

plt.ylim(0.0, 1.75)
plt.xlim(0.0, 5.0)
plt.tight_layout(rect=[-0.035, -0.09, 1.045, 1.09])
plt.savefig(path_to_results + "all_ext_final.png")
plt.savefig(path_to_results + "all_ext_final.pdf")
plt.savefig(path_to_results + "all_ext_final.pgf")
plt.clf()

########## NONBOUND VERSION ##########


def nonbound(choice):
    figure(figsize=(4, 4), dpi=80)

    setup()

    plt.xlabel("steps")
    plt.ylabel("Transmittance")

    bound_xs = []
    bound_vals = []

    bound_xs.append(0)
    bound_vals.append(1.0)

    good_xs = []
    good_vals = []

    good_xs.append(0)
    good_vals.append(1.0)

    bad_xs = []
    bad_vals = []

    bad_xs.append(0)
    bad_vals.append(1.0)

    # 1.3, 0.7, 0.3

    total_steps = 9
    for i in range(1, total_steps):
        bound_xs.append(i)
        good_xs.append(i)
        bad_xs.append(i)
        bound_vals.append(bound_vals[len(bound_vals) - 1])
        good_vals.append(good_vals[len(good_vals) - 1])
        bad_vals.append(bad_vals[len(bad_vals) - 1])
        bound_vals.append(bound_vals[len(bound_vals) - 1]* (1.3 - 1.0) / 1.3)
        good_vals.append(good_vals[len(good_vals) - 1] * (0.7 - 1.0) / 0.7)
        bad_vals.append(bad_vals[len(bad_vals) - 1] * (0.3 - 1.0) / 0.3)
        bound_xs.append(i)
        good_xs.append(i)
        bad_xs.append(i)

    b_x = np.array(bound_xs)
    g_x = np.array(good_xs)
    e_x = np.array(bad_xs)

    b_v = np.array(bound_vals)
    g_v = np.array(good_vals)
    e_v = np.array(bad_vals)

    if(choice == 1):
        plt.plot([0], [0])
        plt.plot(b_x, b_v, linewidth=5.0, label="$\\mu_t = 1.3\\mu_r$")

    if(choice == 2):
        plt.plot([0], [0])
        plt.plot([0], [0])
        plt.plot(g_x, g_v, linewidth=5.0, label="$\\mu_t = 0.7\\mu_r$")


    if(choice == 3):
        plt.plot([0], [0])
        plt.plot([0], [0])
        plt.plot([0], [0])
        plt.plot(e_x, e_v, linewidth=5.0, label="$\\mu_t = 0.3\\mu_r$")

    plt.ylim(-1, 2)
    plt.xlim(0.0, 8.0)
    plt.legend()
    plt.tight_layout(rect=[-0.08, -0.08, 1.08, 1.08])
    if (choice == 1):
        plt.savefig(path_to_results + "bound_ext_behav_final.png")
        plt.savefig(path_to_results + "bound_ext_behav_final.pdf")
        plt.savefig(path_to_results + "bound_ext_behav_final.pgf")
    if (choice == 2):
        plt.savefig(path_to_results + "good_ext_behav_final.png")
        plt.savefig(path_to_results + "good_ext_behav_final.pdf")
        plt.savefig(path_to_results + "good_ext_behav_final.pgf")
    if (choice == 3):
        plt.savefig(path_to_results + "bad_ext_behav_final.png")
        plt.savefig(path_to_results + "bad_ext_behav_final.pdf")
        plt.savefig(path_to_results + "bad_ext_behav_final.pgf")
        
    plt.clf()

nonbound(1)
nonbound(2)
nonbound(3)