#!/bin/bash

export MKL_SERIAL=yes

echo "" | tee -a results.out
echo "Without xCORE-AVX2:" | tee -a results.out
echo "===================" | tee -a results.out

echo "Building ..."
make clean >> build.log 2>&1
make ICC_AVX=0 >> build.log 2>&1

# ./build/bench.org.2560.a 2>&1 | tee -a results.out
./build/bench.tiled.2560.a 2>&1 | tee -a results.out
./build/bench.tiled.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.ref.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.tiled.avx.nta.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.tiled.avx.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.tiled.b64.avx.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.mkl.2560.aligned.a 2>/dev/null | tee -a results.out

echo "" | tee -a results.out
echo "With xCORE-AVX2:" | tee -a results.out
echo "===================" | tee -a results.out

echo "Building ..."
make clean >> build.log 2>&1
make ICC_AVX=1 >> build.log 2>&1

# ./build/bench.org.2560.a 2>&1 | tee -a results.out
./build/bench.tiled.2560.a 2>&1 | tee -a results.out
./build/bench.tiled.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.ref.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.tiled.avx.nta.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.tiled.avx.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.tiled.b64.avx.2560.aligned.a 2>/dev/null | tee -a results.out
./build/bench.mkl.2560.aligned.a 2>/dev/null | tee -a results.out

