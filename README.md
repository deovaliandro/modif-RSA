# Modif RSA dan Hash

## Menjalankan

```bash
g++ main.c -o main
./main
```

## Pembangkitan Kunci
### Pengirim
Misalnya pengirim membangkitkan kunci:

Misalnya dipilih p = 61, q = 53 (keduanya bilangan prima), lalu dipilih d = 1783.
Pasangan kunci yang dihasilkan adalah:

```bash
e = 7, d = 1783, n = 3233
```

dapat dinyatakan sebagai pasangan Kps (kunci publik pengirim) = (7, 3233), dan Kss (kunci privat pengirim) = (1783, 3233).

### Penerima
Misalnya penerima membangkitkan kunci:

Misalnya dipilih p = 17, q = 11 (keduanya bilangan prima), lalu dipilih d = 23.
Pasangan kunci yang dihasilkan adalah:

```bash
e = 7, d = 23, n = 187
```

dapat dinyatakan sebagai pasangan Kpr (kunci publik penerima) = (7, 187), dan Ksr (kunci privat penerima) = (23, 187).

## Enkripsi dan Penandatanganan

Misalnya input = `HALLO`

Lalu input Kpr dan Kss

Hasilnya:
`001e005600200020008b009b0814009f05720198019805720779009b009f005c0078081400bb030a00bb030a030a030a009b06350814030a009f009b0ad0009b0572009f007808140c32057201980146009f009f01460ad00078`

## Dekripsi dan Verifikasi

Input = `001e005600200020008b009b0814009f05720198019805720779009b009f005c0078081400bb030a00bb030a030a030a009b06350814030a009f009b0ad0009b0572009f007808140c32057201980146009f009f01460ad00078`

Lalu input Ksr dan Kps

Hasilnya:
`Message = HELLO`

Hash dari cipher = `c65f99f8c5376adadddc46d5cbcf5762f9e55eb7`

Hash dari pesan yang didekripsi = `c65f99f8c5376adadddc46d5cbcf5762f9e55eb7`

Hal ini menandakan pesan tidak berubah.

*Berhasil*

Bukti:

![hasilrunning](/img/berhasil.png)