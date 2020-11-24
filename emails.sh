# https://www.codechef.com/OSPA2020/problems/EMAIL
# Email names
#! /bin/bash

declare -a users
declare -a domains
regex="^[a-zA-Z][a-zA-Z0-9]*@[a-zA-Z][a-zA-Z0-9]*.[a-z]*$"
i=0
read -r n
while [[ $i -lt $n ]]
do
	read id
	if [[ $id =~ $regex ]]
	then
		IFS='@'
		read -a initial <<< "$id"
		users=("${users[@]}" "${initial[0]}")
		
		IFS='.'
		read -a extension <<< "${initial[1]}"
		domains=("${domains[@]}" "${extension[0]}")
	fi
	i=$(( $i+1 ))
done

# echo everything covert to lower case and sort uniques
users=($(printf "%s\n" "${users[@]}" | tr '[:upper:]' '[:lower:]' | sort -u | tr '\n' ' '))
domains=($(printf "%s\n" "${domains[@]}" | tr '[:upper:]' '[:lower:]' | sort -u | tr '\n' ' '))

echo "${users[@]}"
echo "${domains[@]}"
