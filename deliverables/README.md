# webserver

Toolbenchmark

Toolbenchmark yang saya gunakan adalah JMeter salah satu tools open sorce yang dikembangkan oleh apache pure java aplikasi untuk mengetes beban dan mengukur performa khususnya web aplikasi.JMeter yang saya gunakan adalah JMeter versi 2.11.20151206 yang berjalan di platform linux/ubuntu

Cara melakukan test (soal no 1)
- Install JMeter di lingkungan linux
- Install Jdk 7/jre 7 (min required)
- Install Apache dan Nginx web server di port yang berbeda (hanya hidupkan salah satu jika ingin digunakan) dalam kasus ini saya menginstall Apache di port 8888 sedangkan Nginx di port 80 (default)
- Siapkan file berukuran 500B dan 20KB berekstensi html dan letakkan di var/www/html
di kasus ini saya sediakan file bernama test20.html dan test500.html dan meletakannya di var/www/html/apache dan var/www/html/nginx.
- Hidupkan salah satu web server (Apache atau Nginx) dan disable yang lainnya
- buka JMeter dan buat thread group baru
- Tambahkan di kolom Number of Thread (User) sebanyak 10000
- Klik kanan thread group yang sudah dibuat dan buat sample baru yaitu HTTP request 
- Kemudian tambahkan juga listener untuk thread group tersebut , pilih View Results in Table
- Setelah itu setting url yang akan diakses di tab HTTP Request dalam kasus ini saya membagi ke 2 URL berbeda tiap web server, jika Apache saya akan akses http://localhost:8888/apache/[filetesthtml] dan Nginx http://localhost:80/nginx/[filetesthtml] contohnya jika saya ingin mengakses file html 500B di apache URl yang saya akses adalah http://localhost:8888/apache/test500.html
- jalankan JMeter dan lihat di view listener table hasil dari pemanggilan
- dapat dilihat waktu response setiap request dan average time nya
- di terminal ketikkan top, dan lihat berapa resource memory yang sudah digunakan oleh JMeter
Berikut hasil uji coba kami
- Apache Webserver dengan file 500B

- Apache Webserver dengan file 20KB

- Nginx Webserver dengan file 500B

- Nginx Webserver dengan file 20KB

Terlihat dari hasil tersebut Apache lebih memerlukan waktu yang lama dalam penanganan request namun resource memory yang digunakan lebih sedikit dari Nginx

# Daftar File dan Folder

README.MD	-->	File readme ini
pages/		-->	Directory tempat file html
dir/		-->	Directory tempat file html (alternatif dari pages/ )
custwebserv.c	-->	Source code
Makefile	-->	Script untuk kompilasi
custwebserv	-->	Executables

# Petunjuk Instalasi:

Kompilasi dengan
	
	$ make

Pastikan sudah terlebih dahulu memiliki library dan kompiler c yang lengkap

Jalankan program dengan

	$ ./custwebserv

# Petunjuk menjalankan program:

Masukkan nomor port valid saat diminta, contoh: 12345
Masukkan nama directory untuk mengambil data html, contoh: "pages" (tanpa tanda kutip)
Buka browser dan masukkan ke dalam address bar "localhost:port"




