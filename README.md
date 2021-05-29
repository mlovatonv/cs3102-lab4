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

#### Apollonian gasket

Original

![](images/apollonian_gasket.png)

Recovered

![](images/apollonian_gasket2.png)

```
Compression ratio = 1439769 / 852266 = 1.68
```

#### Balloons

Original

![](images/balloons.png)

Recovered

![](images/balloons2.png)

```
Compression ratio = 1209029 / 7240097 = 0.17
```

#### DLA

Original

![](images/dla.png)

Recovered

![](images/dla2.png)

```
Compression ratio = 2439271 / 2879562 = 0.85
```
