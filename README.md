# Lab 4

## Getting started

```bash
make IMAGE_NAME="balloons"
```

This command will create a `QuadTree` based on the [image](images/balloons.pgm), save the [representation](images/balloons.pgm.qd) of the tree to disk and recreate the [image](images/balloons2.pgm.qd) using the compressed file.

We can use the compression ratio to the evaluate implementation.

```
Compression ratio = Image size (bytes) / Compressed image size (bytes)
```

## Examples

#### Balloons

Original

![](images/balloons.png)

Recovered

![](images/balloons2.png)

```
Compression ratio = 1209029 / 7240097 = 0.17
```

#### Apollonian gasket

Original

![](images/apollonian_gasket.png)

Recovered

![](images/apollonian_gasket2.png)

```
Compression ratio = 1439769 / 852266 = 1.68
```
