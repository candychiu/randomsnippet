
using DataStructures
using StatsBase

# Problem 1
nWashers = 5
nTrials = 1000000
success = 0
for i in 1:nTrials
    rn = rand(1:nWashers,nWashers)
    if get(countmap(rn),1,0) >= 4
        success+=1
    end
end
success/nTrials

# Problem 2
wait1 = 5
wait2 = 7
nTrials = 1000000
success = 0
for i in 1:nTrials
    rn = rand(2)
    if (0 <= (rn[2]-rn[1])*30 <= wait1) ||
       (0 <= (rn[1]-rn[2])*30 <= wait2)
        success+=1
    end
end
success/nTrials

# Problem 3
# nCars=3;  0.6666666666644826
# nCars=4;  0.6667395
# nCars=5;  0.6663580000064863
# nCars=6;  0.6665606666653675
# nCars=7;  0.6665282857209788
# nCars=8;  0.66674625
# nCars=9;  0.6667986666646687
# nCars=10; 0.6665119999960215
# nCars=11; 0.6665550909138054
# nCars=12; 0.6666493333327062
# nCars=20; 0.6666518999954775
# nCars=30; 0.6665245333273143

nCars = 7
nTrials = 1000000
success=0
for i in 1:nTrials
    rn = rand(nCars)    
    rn = sort(rn)
    cn = zeros(nCars)
    cn[1]=2
    cn[nCars]=nCars-1
    for j in 2:nCars-1
        if rn[j+1]-rn[j] >= rn[j]-rn[j-1]
            cn[j]=j-1
        else
            cn[j]=j+1
        end        
    end
    cnTotal = 0
    j = 1    
    while j <= nCars-1
        if (cn[j]==j+1 && cn[j+1]==j)
            j+=2
            cnTotal+=2
        else
            j+=1
        end
    end
    success+=cnTotal/nCars
end
success/nTrials
