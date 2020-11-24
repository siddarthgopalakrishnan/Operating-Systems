# https://www.codechef.com/OSPA2020/problems/DPTINF
# Department Information
#! /bin/bash

deps=0
m=0
f=0
i=0
declare -A dept_map
read -r n
while [[ $i -lt $n ]]
do
	read name d age sex
	if [[ ${dept_map[$d]+_} ]]
	then
		dept_map[$d]=$(( ${dept_map[$d]}+1 ));
	else
		dept_map[$d]+=1
		deps=$(( $deps+1 ))
	fi
	#echo "Dept $d count : ${dept_map[$d]}"
	if [[ $sex == "F" ]]
	then
		f=$(( $f+1 ))
	else
		m=$(( $m+1 ))
	fi
	
	i=$(( $i+1 ))
done

mx=0
max_dept=""
for d in "${!dept_map[@]}"
do
	#echo "Dept $d count : ${dept_map[$d]}"
	if [[ ${dept_map[$d]} -gt $mx ]]
	then
		mx=${dept_map[$d]}
		max_dept=$d
	fi
done

echo "$deps"
echo "$m $f"
echo "$max_dept"
