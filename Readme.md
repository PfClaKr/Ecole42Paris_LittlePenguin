# Little penguin

## Assignment00

##### Linus Torvalds, 리눅스와 깃 창시자. 대단한 핀란드사람
> 하루이내의 최근 코밋이 있는...
> https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/

##### ```apt install build-essential libncurses-dev bison flex libssl-dev libelf-dev```
##### ```make menuconfig``` ncurses을 사용하여 아주멋진 .config파일 작성가능
> .config 안에서\
>```# CONFIG_DEBUG_INFO_BTF is not set```\
>```CONFIG_LOCALVERSION_AUTO=y```\
>```CONFIG_LOCALVERSION="ychun" ```
```sh
make modules_install install
sudo reboot
uname -r
```
~~virtualbox 디스크 50gb는 잡고하셔야 마음이 편하실껍니다~~
## Assignment01
모든 커널 모듈에는 최소 두 개의 함수필요.\
하나는 설치할 때, 하나는 제거할 때.
> [v2.3 이전시대 유물](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/module.h?h=v5.7#n75)\
> 우리는 어떤 시스템에서나 complie이 되어야하기 때문에...
### *printk(const char *ftm, ...)*
커널에 **printk()**는 **printf()**와 닮았지만 사용자공간을 위해 만들지않음.\
standardout에 작성하지않고 로그작성을 위해 쓰임.\
[중요순위 매크로](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/kern_levels.h?h=v5.7)
> ```printk(KERN_INFO "Hello world!\n")```   
> C complier lexer가 가까이 있는 리터럴을 결합   
> ```printk("\001 6 Hello world!\n")```

[메이크파일 Kbuild](https://www.kernel.org/doc/html/latest/kbuild/makefiles.html)\
obj-m이 loadable kernel module임을 알려줌. 흔한 complie처럼 ***.c**, ***.s** 까지왔다가 ***.ko**로 컴파일함.
```sh
sudo insmod main.ko
dmesg | tail -1 
sudo rmmod main.ko
```
서브젝트처럼 날짜보이고싶으면
```sh
dmesg -T --color=always
```
참고로, 기본적으로 보여주는 커널로그 레벨을 보고싶으면
```sh
cat /proc/sys/kernel/printk
7	4	1	7
```
첫번부터 콘솔로그, 기본로그, 최소로그, 부팅시 기본로그이다.
```sh
sudo echo 'LOGLEVEL="6"' >> /etc/sysconfig/console
```
으로 콘솔로그를 원하는 레벨로 맞출 수 있다.
## Assignment02
##### ```cp Makefile Makefile.orig```
아주 쉽게 EXTRAVERSION을 찾을 수 있다.\
00때와같이 빌드해주자.\
~~얘땜에 50gb도 다채웠다가 /usr/module에서 옛날커널 모듈 지움~~\
[수정패치 규칙](https://www.kernel.org/doc/html/v4.17/process/submitting-patches.html#diff-up)에 맞춰서 makefile 수정사항을 기록.
```sh
diff -u Makefile.orig Makefile > Makefile.patch
```
## Assignment03
00에서 받았던 git폴더안에 linux커널코딩스타일 검사해주는 스크립트가 있다.
```sh
<linux_kernel_raw_source_path>/scripts/checkpatch.pl main.c -no-tree -file --strict
```
리눅스 커널 코딩 스타일 저자가 매우 재미있으니 [원문](https://www.kernel.org/doc/html/latest/process/coding-style.html)을 읽는것을 매우매우 추천.
```
First off, I’d suggest printing out a copy of the GNU coding standards, and NOT read it. Burn them, it’s a great symbolic gesture.
```
~~직접해볼까 고민한 1인~~
## Assignment04
01에서 작성했던 간단한 커널모듈을 가지고 usb keyboard가 인식되었을때 작동하는 모듈을 작성해야한다.
유저스페이스 핫플러그는 udev를 기준으로 풀이하겠다. [출처](https://linuxsleuthing.blogspot.com/2012/08/obtaining-usb-device-details.html)는 여기.
```bash
$ lsusb -D /dev/bus/usb/001/001
Device: ID 1d6b:0002 Linux Foundation 2.0 root hub
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass            9 Hub
  bDeviceSubClass         0 
  bDeviceProtocol         1 Single TT
  bMaxPacketSize0        64 
  ...
```
lsusb로 usb의 descriptor을 확인해서 정보를 얻거나,
```bash
$ udevadm monitor
KERNEL[28004.073393] add      /devices/LNXSYSTM:00/LNXSYBUS:00/ACPI0004:00/VMBUS:00/dc4ec6d5-5597-11ef-8a45-085bd6c6f50b (vmbus)
KERNEL[28004.074159] bind     /devices/LNXSYSTM:00/LNXSYBUS:00/ACPI0004:00/VMBUS:00/dc4ec6d5-5597-11ef-8a45-085bd6c6f50b (vmbus)
...
```
udevadm monitor 명령어로 커널 이벤트를 직접 확인해가며 정보를 얻을 수도 있다. \
\
/etc/udev/rules.d/ 에 udev관련 유저용 규칙파일을 넣을수있다. [자세한 규칙파일(.rules) 문법은 여기에서](https://www.reactivated.net/writing_udev_rules.html).

모든 usb 디바이스는 종류, 제조사 버전 등등 자신의 정보를 담은 descriptor을 가지고있다. 아까 lsusb로 봤던 Device Descriptor가 그런 내용인 것이다. 그중에서 우리는 bInterfaceProtocol을 써서 usb-keyboard만 걸러주도록하겠다. \
\
또한 .rules는 맨앞에 붙는 숫자로 우선순위가 결정된다. 10이 제일 높은 우선순위, 99가 제일 낮은 우선순위이다.
```sh
udevadm control --reload-rules
dmesg | tail
```
규칙파일을 적용시켜주고 로그를 확인해보자.
## Assignment05
misc char device driver을 작성, write와 read를 커스텀하는 서브젝트이다.

리눅스는 Unix 시절부터 디바이스 별로 번호를 붙여 관리했다. Major number와 Minor number가 있는데, 자세한내용은 [공식문서](https://www.kernel.org/doc/Documentation/admin-guide/devices.txt)을 참고해보자. \

리눅스에 miscdevice를 등록하기 위해서는 여러 작업이 필요한데, 일단 크게 file_operations을 커스텀 해야된다.
```c
/* include/linux/fs.h */
struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	...
```
[linux/fs.h](https://lwn.net/2001/0906/a/fs.h.php3#:~:text=struct%20file_operations%20%7B) 안에 정의 되어있는 file_operations을 커스텀하여서 misc device을 읽을 때, 쓸 때, 열 때 등등 여러가지를 커스터마이징 할 수 있다.
```c
static struct file_operations ft_fops = {
  .read = ft_read,
  .write = ft_write,
};
```
식으로 만들어주고,
```c
static struct miscdevice device = {
  .fops = &ft_fops;
};
```
정도로 적용시키면 되겠다. 참고로 miscdevice는 [이렇게](https://android.googlesource.com/platform/external/kernel-headers/+/froyo/original/linux/miscdevice.h)생겼다.

커널과 유저는 서로 사용하는 영역이 다르다. 그러므로 유저의 인풋을 받거나 아웃풋을 커널에서 보내줘야 한다면 특별한 함수들을 써야한다. 서브젝트가 요구하는건 misc device의 read와 write이므로,
```c
/* fs/libfs.c */
ssize_t simple_write_to_buffer(void *to, size_t available, loff_t *ppos, const void __user *from, size_t count)
ssize_t simple_read_from_buffer(void __user *to, size_t count, loff_t *ppos, const void *from, size_t available)
```
[커널이미지](https://elixir.bootlin.com/linux/v5.17/source/fs/libfs.c#L756)에서 지원하는 함수를 쓰도록 하겠다. \
테스트 할 때 유의할점은, fd하나로 read와 write을 같이 진행하게 되면, loff_t *ppos 값을 같이 쓰게 되므로 의도한대로 작동하지 않을수 있다. 예를들어
```c
int fd = open("42", O_RDWR);
char buf[6];

write(fd, "ychun", 5);
int len = read(fd, buf, 5);
buf[len] = '\0';
```
위와 같은 코드를 돌릴 때 나의 기대값은 buf안에 ychun이 들어있는 거지만, 실제로는 write하면서 ppos값이 늘어나 read는 내가 의도치않은 곳을 읽게된다. 이것을 방지하기위해선 close()나 lseek()같이 ppos값을 초기화 할 필요가 있다.
## Assignment06
linux-next커널을 빌드, 부트하면 된다.
```sh
git clone git://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git
```
Assignment00, 02와 같은 방식으로 빌드한다.

배포되고 있는 Linux가 릴리즈버전이라면, Linux-next는 베타버전이라고 이해하면 편하다. 회사라면 dev브랜치를 펼치고 관리하겠지만 크기가 크기인만큼 다른 git을 사용하는것처럼 보인다.
~~서로 백만개가 넘는 커밋이 있는건 비밀~~
## Assignment07
## Assignment08
## Assignment09
[](https://www.reddit.com/r/linuxquestions/comments/1al9ki2/how_do_you_get_a_complete_boot_log/)