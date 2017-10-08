#!/bin/bash


timestamp=$(date +%m-%d-%Y-%T)



echo "									"
echo "   ##################################################################"
echo "   #                                                                #"
echo "   #               ECE-585 Microprocessor system design             #"
echo "   #            Final project : Last level cache simulation         #"
echo "   #                       $timestamp                      #"
echo "   #                                                                #"
echo "   # Team members                                                   #"
echo "   # - Amit Solapurkar                                              #"
echo "   # - Kalyani Chawak                                               #"
echo "   # - Soma Sai Charitha                                            #"
echo "   # - Haranadh Chintapalli                                         #"
echo "   #                                                                #"
echo "   #                                                                #"
echo "   ##################################################################"
echo "									"

make clean
echo "									"
make
echo "									"


## check the trace file
echo "Checking the trace file for invalid inputs"
grep -E '^[0-689][[:space:]][[:xdigit:]]{1,8}$' $1 -v

touch $1_result_$timestamp

./cachebuild $1 > $1_result_$timestamp


file_nam=$(ls -rt|tail -n 1)

echo "									"
echo "#################################################################"
echo "            Cache statistics                                "
cat $file_nam
echo "#################################################################"
echo "									"

echo "  Results are captured in file --->   $file_nam "


echo "									"
echo "  Check README file for more options			"
echo "									"
