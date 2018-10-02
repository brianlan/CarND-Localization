def normalize(l):
    sum_value = sum(l)
    return [i / sum_value for i in l]


def sense(p_prev, Z, p_hit):
    p_posterior = [float(Z == cell) * p_hit[1] + float(Z != cell) * p_hit[0] for cell in world]
    p_cur = normalize([i * j for i, j in zip(p_prev, p_posterior)])
    return p_cur


def move(p, U):
    u = U % len(p)
    return p[-u:] + p[:-u]


def inaccurate_move(p, U, undershot=.1, overshot=.1):
    n_locs = len(p)  # num locations
    move_uncertainty = [undershot, 1 - undershot - overshot, overshot]
    _p = [0.] * n_locs
    for i in range(n_locs):
        move_to_positions = [(i + U - 1) % n_locs, (i + U) % n_locs, (i + U + 1) % n_locs]
        # print("{} => {}".format(i, move_to_positions))
        for j, pos in enumerate(move_to_positions):
            _p[pos] += p[i] * move_uncertainty[j]

    return _p


world = ['green', 'red', 'red', 'green', 'green']
p = [1. / len(world)] * len(world)
# p = [0, 1, 0, 0, 0]
p_hit = [.2, .6]
measurements = ['red', 'green']
motions = [1, 1]


# for m in measurements:
#     p = sense(p, m, p_hit)
#     print(p)

# for i in range(10):
#     print(i - 5, p, '=>',  move(p, i - 5))

for U, Z in zip(motions, measurements):
    p = sense(p, Z, p_hit)
    p = inaccurate_move(p, U, undershot=.1, overshot=.1)

print(p)



