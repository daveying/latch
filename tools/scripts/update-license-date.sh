from=$1
to=$2
prefix="Copyright (c)"
suffix="Xingpeng Da"
echo "Changing license date from ${from} to ${to}..."
grep -rlZ "${prefix} ${from} ${suffix}" . | xargs -0 sed -i "s/${prefix} ${from} ${suffix}/${prefix} ${to} ${suffix}/g"
