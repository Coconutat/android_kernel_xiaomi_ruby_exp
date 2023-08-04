#!/bin/bash
if [ -d KernelSU ];
then
    echo "Find KernelSU Floder,will remove it"
	rm -rf KernelSU
	rm -rf drivers/kernelsu
else
	echo "Don't find KernelSU Floder,good."
fi

curl -LSs "https://raw.githubusercontent.com/tiann/KernelSU/main/kernel/setup.sh" | bash -s main