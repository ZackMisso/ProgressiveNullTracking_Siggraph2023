import matplotlib.pyplot as plt
import math
import random
from random import random

import matplotlib as mpl
# import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams

from matplotlib.pyplot import figure


def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Libertine'
    #mpl.rc('font', family=fontName)
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
    #mpl.rc('text', color=textColor)
    mpl.rc('text', hinting='native')
    siz = 14  # 7.97
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


figure(figsize=(0.75, 4), dpi=80)

setup()

# data = open("tele_var_reduc_paper.txt", "r").readlines()

# shader_scene
# max_tick = 50.0
# min_tick = -50.0

# img_path = "/Users/corneria/Desktop/cmaps/paper_maps/vert_prinsenvlag.png"
# img = plt.imread(img_path)

# xpos = []
# ypos = []

# plt.grid(False)
# plt.xticks([])
# plt.imshow(img, extent=[min_tick/12.8, max_tick/12.8, min_tick, max_tick])

# plt.tight_layout(rect=[-0.0, -0.06, 1.4, 1.05])
# plt.savefig("/Users/corneria/Desktop/cmaps/paper_maps/vert_prinsenvlag_map.png")
# plt.savefig("/Users/corneria/Desktop/cmaps/paper_maps/vert_prinsenvlag_map.pgf")

# consist_scene
# max_tick = 1.0
# min_tick = -1.0

img_path = "/Users/corneria/Desktop/cmaps/paper_maps/vert_prinsenvlag.png"
img = plt.imread(img_path)

xpos = []
ypos = []

max_tick = 128
min_tick = 0
plt.grid(False)
plt.xticks([])
plt.yticks([0, 32, 64, 96, 128], ["-1.0", "-0.5", "0.0", "0.5", "1.0"])
plt.imshow(img, extent=[min_tick/12.8, max_tick/12.8, min_tick, max_tick])

plt.tight_layout(rect=[-0.0, -0.06, 1.35, 1.05])
plt.savefig(
    "/Users/corneria/Desktop/cmaps/paper_maps/vert_prinsenvlag_map.png", transparent=True)
plt.savefig(
    "/Users/corneria/Desktop/cmaps/paper_maps/vert_prinsenvlag_map.pgf", transparent=True)


# ao_scene
# figure(figsize=(4, 0.75), dpi=80)

# setup()

# max_tick = 128
# min_tick = 0

# img_path = "/Users/corneria/Desktop/cmaps/paper_maps/horiz_prinsenvlag.png"
# img = plt.imread(img_path)

# xpos = []
# ypos = []

# plt.grid(False)
# plt.yticks([])
# plt.xticks([0, 32, 64, 96, 128], ["-0.003", "-0.0015", "0.00", "0.05", "0.10"])
# # plt.set_xlabels([-0.002, -0.001, 0.0, 0.06, 0.12])
# # plt.imshow(img, extent=[min_tick, max_tick, min_tick/12.8, max_tick/12.8])
# plt.imshow(img, extent=[min_tick, max_tick, min_tick/12.8, max_tick/12.8])

# plt.tight_layout(rect=[-0.0, -0.0, 1.0, 1.0])
# # plt.tight_layout()
# plt.savefig(
#     "/Users/corneria/Desktop/cmaps/paper_maps/horiz_prinsenvlag_map.png", transparent=True)
# plt.savefig(
#     "/Users/corneria/Desktop/cmaps/paper_maps/horiz_prinsenvlag_map.pgf", transparent=True)

# 100x1280
