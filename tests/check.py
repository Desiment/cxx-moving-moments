MEANS_TEST   = True
RAW_TEST     = True
CENTRED_TEST = True
ORDER_TEST   = True

######### Quasi-Arithmetic Means ###########
if (MEANS_TEST):
    print("========= MEANS ==========")
    raw_answers = []
    with open("means.a") as file:
        for line in file:
            raw_answers.append(list(map(float, line.split(',')[:-1])))
    
    raw_querry = []
    with open("means.q") as file:
        for line in file:
            raw_querry.append(list(map(float, line.split(',')[:-1])))
    
    name = ["arithmetic", "geometric", "harmonic", "squared"]
    for (q, a) in zip(raw_querry, raw_answers):
        for j in range(4):
            print(f'Differnce in {name[j]} mean : ', q[j] - a[j])
        print("---")


######## Raw moments ###########
if (RAW_TEST):
    print("========= ORIGIN MOMENTS ==========")
    raw_answers = []
    with open("raw.a") as file:
        for line in file:
            raw_answers.append(list(map(float, line.split(',')[:-1])))
    
    raw_querry = []
    with open("raw.q") as file:
        for line in file:
            raw_querry.append(list(map(float, line.split(',')[:-1])))
    
    for (q, a) in zip(raw_querry, raw_answers):
        for j in range(5):
            print("Differnce in raw moment of order ", j, " : ", q[j] - a[j])
        print("---")

####### Centred momemnts ##########
if (CENTRED_TEST):
    print("========= CENTRED MOMENTS ==========")
    moment_answers = []
    with open("moments.a") as file:
        for line in file:
            moment_answers.append(list(map(float, line.split(',')[:-1])))

    moment_querry = []
    with open("moments.q") as file:
        for line in file:
            moment_querry.append(list(map(float, line.split(',')[:-1])))
    
    for (q, a) in zip(moment_querry, moment_answers):
        for j in range(5):
            print("Differnce in centred moment of order ", j, " : ", q[j] - a[j])
        print("---")


####### Centred momemnts ##########
if (ORDER_TEST):
    print("========= ORDER STATISTICS ==========")
    stats_answers = []
    with open("stats.a") as file:
        for line in file:
            stats_answers.append(list(map(float, line.split(',')[:-1])))

    stats_querry = []
    with open("stats.q") as file:
        for line in file:
            stats_querry.append(list(map(float, line.split(',')[:-1])))

    for (q, a) in zip(stats_querry, stats_answers):
        for j in range(5):
            print("Differnce in order-statistics of order ", j/4, "% : ", q[j] - a[j])
        print("---")
