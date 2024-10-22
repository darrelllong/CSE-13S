BEGIN   { sum=0}
        { sum=sum+$1}
END     { mean=sum/NR; print mean }
