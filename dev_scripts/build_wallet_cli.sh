# echo $'Removing old build...\n'
# rm -rf /monero/build
# mkdir /monero/build
cd /monero/build
echo $'\nBuilding monero-wallet-cli...\n'
if [ ! -d "bin" ]; then
  # Control will enter here if bin doesn't exist.
  cmake .. -DMANUAL_SUBMODULES=1
fi
make simplewallet
echo $'Built wallet is in:'
echo $(pwd)/bin/