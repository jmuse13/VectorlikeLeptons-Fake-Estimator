#! /bin/bash

do_systematics=0
do_systematic_fake=1
mc_campaigns="a d e"
data_campaigns="1516 17 18"

start_time=`date +%s`

# "MC16X" gets replaced with "MC16a,MC16d,MC16e" and dataX gets replaced with "data1516,data17,data18"
bkg_ntuple_inputs=(
/raid06/users/muse/EXOT19_aug27/input/MC16X/diboson_MC16X/
/raid06/users/muse/EXOT19_aug27/input/MC16X/multiboson_MC16X/
/raid06/users/muse/EXOT19_aug27/input/MC16X/top_MC16X/
/raid06/users/muse/EXOT19_aug27/input/MC16X/W+jets_MC16X/
/raid06/users/muse/EXOT19_aug27/input/MC16X/Z+jets_MC16X/)
data_ntuple_inputs=(
/raid06/users/muse/EXOT19_aug27/input/dataX )

function limit_background_jobs () {
while [ `pgrep -u $USER makeVLLFFs | wc -l` -gt $( echo "`nproc` + 1" | bc) ]; do
    sleep $(echo $RANDOM" % 10" | bc -l)
done
sleep 1
}

function basename_with_dotroot_if_needed () {
echo `basename $1``[ $(basename $1 | rev | cut -d"." -f 1 | rev) != "root" ] && echo ".root"`
}

bin_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
out_dir=output_eventloop_`date +%F-%s`
mkdir -p $out_dir/fake_el
out_dir=`readlink -f $out_dir`

data_outfiles=""
echo "Processing data ..."
for i in "${data_ntuple_inputs[@]}"; do
    datasample1="$(cut -d '/' -f7 <<<"$i")"
    sample="$(cut -d '1' -f1 <<<"$datasample1")"
    (
    for campaign in `echo $data_campaigns`; do
        this_campaign_outputs=()
        echo $campaign
        i_camp=`echo $i | sed "s/dataX/data$campaign/g"`
        mkdir $out_dir/`basename $i_camp`
        for input in `ls $i_camp`; do
            echo $input
            limit_background_jobs
            output=$out_dir"/"`basename $i_camp`"/"`basename_with_dotroot_if_needed $input`
            this_campaign_outputs+=( $output )
            nice -n 5 $bin_dir/makeVLLFFs -a -e -d -t 1 -g $sample -p $campaign -i $i_camp"/"$input -o $output&
        done
        wait
        hadd $out_dir"/"`basename_with_dotroot_if_needed $i_camp` `echo ${this_campaign_outputs[@]}`
        data_outfiles+=( $out_dir"/"`basename_with_dotroot_if_needed $i` )
    done
    ) &
done

background_outfiles=""
echo "Processing backgrounds ..."
for i in "${bkg_ntuple_inputs[@]}"; do
    datasample1="$(cut -d '/' -f8 <<<"$i")"
    sample="$(cut -d '_' -f1 <<<"$datasample1")"
    (
    for campaign in `echo $mc_campaigns`; do
        this_campaign_outputs=()
        i_camp=`echo $i | sed "s/MC16X/MC16$campaign/g"`
    	mkdir $out_dir/`basename $i_camp`
        for input in `ls $i_camp`; do
            echo $input
            limit_background_jobs
            output=$out_dir"/"`basename $i_camp`"/"`basename_with_dotroot_if_needed $input`
            this_campaign_outputs+=( $output )
            nice -n 5 $bin_dir/makeVLLFFs $([ $do_systematics -ne 0 ] && echo "-s") -a -e -t 1 -g $sample -p $campaign -i $i_camp"/"$input -o $output&
        done
        wait
        hadd $out_dir"/"`basename_with_dotroot_if_needed $i_camp` `echo ${this_campaign_outputs[@]}`
        background_outfiles+=$out_dir"/"`basename_with_dotroot_if_needed $i_camp`" "
    done
    ) &
done
wait

roots=$(ls $out_dir/*.root | grep -v data | sed "s/MC16./MC16X/g" | sed "s/ /\n/g" | sort | uniq)
hadded_roots=( $out_dir"/data.root" )
hadd $out_dir"/data.root" `ls $out_dir"/"data*.root` &
for root in $roots; do
    hadd_name=$out_dir"/"`basename $root | rev | cut -d"_" -f 2- | rev`".root"
    hadd $hadd_name $(for campaign in `echo $mc_campaigns`; do echo $root | sed "s/MC16X/MC16$campaign/g"; done) &
    hadded_roots+=( $hadd_name )
done
wait

end_time=`date +%s`
time_diff=`echo $end_time - $start_time | bc`
echo "finished after "$time_diff" seconds."



