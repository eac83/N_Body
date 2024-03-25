from math import pi

h = 0.67
pc = 3.0857e16
H_0 = 100 * 0.67 * 1e3 / (pc * 1e6)
G = 6.67430e-11

rho_crit = 3*H_0**2/(8*pi*G)


Omega_m = 0.3153
rho_m = rho_crit*Omega_m

box_size = 40 * pc * 1e6
n = 1000

mass_dm = rho_m * box_size**3 / n
print(mass_dm)
mass_unit = 1.98847e35
mass_particle = mass_dm/mass_unit

print(mass_particle)