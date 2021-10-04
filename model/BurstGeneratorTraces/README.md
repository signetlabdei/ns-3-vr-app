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
* M. Lecci, M. Drago, A. Zanella, M. Zorzi, "An Open Framework for Analyzing and Modeling XR Network Traffic," in IEEE Access, vol. 9, pp. 129782-129795, 2021. Open Access DOI: [10.1109/ACCESS.2021.3113162](https://doi.org/10.1109/ACCESS.2021.3113162).

NOTE: `.csv` traces are based on acquisitions of real VR traffic streaming. Target data and frame rate are only indicative and may vary over time as well as in average.