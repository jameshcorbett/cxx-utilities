# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
#
# This file does only contain a selection of the most common options. For a
# full list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
import shutil
# sys.path.insert(0, os.path.abspath('.'))

##### Call doxygen in ReadtheDocs #########################################################################
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'
if read_the_docs_build:

    # Make sure directory exists
    cwd = os.getcwd()

    build_path = os.path.join(cwd, "_build")
    if not os.path.isdir(build_path):
        os.mkdir(build_path)

    html_path = os.path.join(build_path, "html")
    if not os.path.isdir(html_path):
        os.mkdir(html_path)

    docs_path = os.path.join(cwd, "docs", "doxygen")
    common_path = cwd

    doxyfile_src = os.path.join(docs_path, "Doxyfile.in")
    doxyfile_dst = os.path.join(build_path, "Doxyfile")
    config_src = os.path.join(docs_path, "LvArrayConfig.hpp")
    config_dst = os.path.join(common_path, "LvArrayConfig.hpp")

    input_dirs = ["."]

    # Write correct ReadtheDocs path and input directories
    shutil.copy(doxyfile_src, doxyfile_dst)
    with open(doxyfile_dst, "a") as f:
        f.write("\nINPUT = %s" % " ".join(input_dirs))
        f.write("\nOUTPUT_DIRECTORY = %s/doxygen_output" % html_path)
        f.write("\nHAVE_DOT = YES")

    # Make a symlink to LvArrayConfig.hpp in src
    if not os.path.exists(config_dst):
        os.symlink(config_src, config_dst)

    # Call doxygen
    from subprocess import call
    call(['doxygen', doxyfile_dst])
###########################################################################################################



# -- Project information -----------------------------------------------------

project = u'SimUtils'
copyright = u'2018, Lawrence Livermore National Laboratory'
author = u'Randolph Settgast'

# The short X.Y version
version = u''
# The full version, including alpha/beta/rc tags
release = u''


# -- General configuration ---------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.mathjax',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path .
exclude_patterns = [u'_build', 'Thumbs.db', '.DS_Store']

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
#html_theme = 'alabaster'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Custom sidebar templates, must be a dictionary that maps document names
# to template names.
#
# The default sidebars (for documents that don't match any pattern) are
# defined by theme itself.  Builtin themes are using these templates by
# default: ``['localtoc.html', 'relations.html', 'sourcelink.html',
# 'searchbox.html']``.
#
# html_sidebars = {}


# -- Options for HTMLHelp output ---------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'SimUtilsdoc'


# -- Options for LaTeX output ------------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'SimUtils.tex', u'SimUtils Documentation',
     u'Randolph Settgast', 'manual'),
]


# -- Options for manual page output ------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'SimUtils', u'SimUtils Documentation',
     [author], 1)
]


# -- Options for Texinfo output ----------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'SimUtils', u'SimUtils Documentation',
     author, 'SimUtils', 'One line description of project.',
     'Miscellaneous'),
]


# -- Extension configuration -------------------------------------------------
