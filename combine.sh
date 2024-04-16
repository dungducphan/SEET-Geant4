#!/usr/bin/env bash

if [ ! -d "Summary" ]; then
  mkdir Summary
fi

numberOfParticles=1E7
arrayOfEnergy=(1000 0500 0200 0100 0050 0020 0010)

for i in "${arrayOfEnergy[@]}"
do
  # Write the macro file
  touch ./run_"${i}"MeV.mac
  {
  echo "/run/numberOfThreads 48"
  echo "/run/initialize"
  echo "/gps/ene/mono ${i} MeV"
  echo "/run/beamOn ${numberOfParticles}"
  } >> ./run_"${i}"MeV.mac

  # Run the simulation
   ./SEET ./run_"${i}"MeV.mac

  # Combine the output files
  hadd -f Trans_"${i}"MeV_"${numberOfParticles}".root ./Ana_RunID_000_t*.root

  # Remove the output files and the config file
  rm ./Ana_RunID_000_t*.root
  rm ./run_"${i}"MeV.mac
done





