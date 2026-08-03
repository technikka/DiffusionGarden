# Gray-Scott Experiments

### Coral

<img src="images/coral.png" alt="Celtic reaction-diffusion pattern" width="400">

*Image Settings:* <br>
rows = 300 <br>
columns = 300 <br>

#### Parameters

diffusion_a = 0.2 <br>
diffusion_b = 0.1 <br>
feed_rate = 0.055 <br>
kill_rate = 0.062 <br>

seed_a = 0.0 <br>
seed_b = 1.0 <br>
seed_size = 3x3 cells <br>


#### Notes
Branching coral-like ridges with eightfold radial symmetry.

---


### Totem

<img src="images/totem.png" alt="Celtic reaction-diffusion pattern" width="400">

*Image Settings:* <br>
rows = 300 <br>
columns = 300 <br>


#### Parameters

diffusion_a = 0.2 <br>
diffusion_b = 0.1 <br>
feed_rate = 0.035 <br>
kill_rate = 0.062 <br>

seed_a = 0.0 <br>
seed_b = 1.0 <br>
seed_size = 3x3 cells <br>


#### Notes
Glyph-like radial pattern composed of curved segments and scattered circular nodes.

<br>

## Four-Seed Convergence

Adding four evenly spaced seeds near the center of the simulation, I expected the resulting patterns to collide chaotically.

Instead, as the four growing regions converged, they appeared to slow or halt just before making contact. They then continued expanding inward toward the center while remaining separated.

Once the central space filled in, the pattern continued expanding outward much like the original one-seed version, as though the four seeds had been parts of one organism all along.



<p align="center">
  <img src="images/1-seed-coral.png" alt="Coral pattern from a single seed" width="48%"><img src="images/4-seed-coral.png" alt="Coral pattern from four seeds" width="48%">
  <br>
  <p align="center">
  <em>Coral: Single-seed (left) and four-seed (right) initialization.</em>
</p>
</p>

<br>

<p align="center">
  <img src="images/1-seed-totem.png" alt="Totem pattern from a single seed" width="48%"><img src="images/4-seed-totem.png" alt="Totem pattern from four seeds" width="48%">
  <br>
  <p align="center">
  <em>Totem: Single-seed (left) and four-seed (right) initialization.</em>
</p>
</p>

### Cipher

<img src="images/cipher.png" alt="Celtic reaction-diffusion pattern" width="400">

*Image Settings:* <br>
rows = 1201 <br>
columns = 1201 <br>

#### Parameters

diffusion_a = 0.2 <br>
diffusion_b = 0.1 <br>
feed_rate = 0.05 <br>
kill_rate = 0.06 <br>

seed_a = 0.0 <br>
seed_b = 1.0 <br>
seed_size = 3x3 cells <br>

#### Notes
A porous circular form with clustered openings, resembling an encoded symbol or unfamiliar signal.
