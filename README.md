VR traffic model for ns-3
=========================

**NOTE:** You can find more information about this work in the following paper:

_**Mattia Lecci, Andrea Zanella, Michele Zorzi, "An ns-3 Implementation of a Bursty Traffic Framework for Virtual Reality Sources," accepted to Workshop on ns-3 (WNS3), Jun. 2021, Virtual Event, USA, Pre-print available: [arXiv:2103.04609](https://arxiv.org/abs/2103.04609)**_

Examples of usage for the newly introduced features can be found in the ``examples/`` folder.

## Install

### Prerequisites ###

To run simulations using this module, you will need to install ns-3, and clone
this repository inside the `contrib` directory. 
Required dependencies include git and a build environment.

#### Installing dependencies ####

Please refer to [the ns-3 wiki](https://www.nsnam.org/wiki/Installation) for instructions on how to set up your system to install ns-3.

#### Downloading #####

First, clone the main ns-3 repository:

```bash
git clone https://gitlab.com/nsnam/ns-3-dev ns-3-dev
```

Then, clone the vr-app module:
```bash
git clone https://github.com/signetlabdei/ns-3-vr-app ns-3-dev/contrib/vr-app
```

### Compilation ###

Configure and build ns-3 from the `ns-3-dev` folder:

```bash
./waf configure --enable-tests --enable-examples
./waf build
```

This module does not provide Python bindings at the moment.

### Documentation ###

To compile the documentation, please follow the instructions from the [ns-3 manual](https://www.nsnam.org/docs/manual/html/documentation.html).

Basic steps:

1. Install the documentation-specific dependencies as described in the [ns-3 installation guide](https://www.nsnam.org/wiki/Installation)
1. You might need to fix the ImageMagick permissions for ghostscript files

Compiling standalone documentation:

1. Run in your terminal, in the ns-3 root folder, `make -C contrib/vr-app/doc html`
1. Open with your browser the file `contrib/vr-app/doc/models/build/html/bursty-framework.html` to visualize the documentation of the model
