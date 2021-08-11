Traces present a consistent naming scheme: `{appName}_{dataRate}mbps_{frameRate}fps.csv`.
Specifically:
* `appName`: an indication of the name of the streamed application. Specifically,
  - `vp` stands for _Virus Popper_
  - `mc` stands for _Minecraft_
  - `ge_cities` stands for _Google Earth VR - Cities_
  - `ge_tour` stands for _Google Earth VR - Tour_
* `dataRate`: the target data rate
* `frameRate`: the target frame rate

Some additional meta data might be found in the commented header of the `.csv` file.

For further information please check the reference paper:
* M. Lecci, M. Drago, A. Zanella, M. Zorzi, "An Open Framework for Analyzing and Modeling XR Network Traffic," Submitted to IEEE Access, Pre-print available: [arXiv:2108.04577](https://arxiv.org/abs/2108.04577).

NOTE: `.csv` traces are based on acquisitions of real VR traffic streaming. Target data and frame rate are only indicative and may vary over time as well as in average.