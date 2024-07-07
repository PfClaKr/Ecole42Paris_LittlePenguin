# Little penguin

## Assignment00

##### Linus Torvalds, 리눅스와 깃 창시자. 대단한 핀란드사람
> 하루이내의 최근 코밋이 있는...
> https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/

##### ```apt install build-essential libncurses-dev bison flex libssl-dev libelf-dev```
##### ```make menuconfig``` ncurses을 사용하여 아주멋진 .config파일 작성가능
> .config 안에서
> <pre>
> <code>
> # CONFIG_DEBUG_INFO_BTF is not set
> CONFIG_LOCALVERSION_AUTO=y
> CONFIG_LOCALVERSION="ychun"
> </code>
> </pre>
##### ```make modules_install install```
##### ```sudo reboot```
##### ```uname -r```
~~virtualbox 디스크 50gb는 잡고하셔야 마음이 편하실껍니다~~
## Assignment01
모든 커널 모듈에는 최소 두 개의 함수필요.\
하나는 설치할 때, 하나는 제거할 때.
> [v2.3 이전시대 유물](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/module.h?h=v5.7#n75)\
> 우리는 어떤 시스템에서나 complie이 되어야하기 때문에...
### *printk(const char *ftm, ...)*
커널에 **printk()**는 **printf()**와 닮았지만 사용자공간을 위해 만들지않음.\
standardout에 작성하지않고 로그작성을 위해 쓰임.\
[로그작성을 위한 매크로](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/kern_levels.h?h=v5.7)
> ```printk(KERN_INFO "Hello world!\n")```   
> C complier lexer가 가까이 있는 리터럴을 결합   
> ```printk("\001 6 Hello world!\n")```

[메이크파일 Kbuild](https://www.kernel.org/doc/html/latest/kbuild/makefiles.html)\
obj-m이 loadable kernel module임을 알려줌. 흔한 complie처럼 ***.c**, ***.s** 까지왔다가 ***.ko**로 컴파일함.
<pre>
<code>
sudo insmod main.ko
dmesg | tail -1 
sudo rmmod main.ko
</code>
</pre>
## Assignment02
##### ```cp Makefile Makefile.orig```
아주 쉽게 EXTRAVERSION을 찾을 수 있다.\
00때와같이 빌드해주자.\
~~얘땜에 50gb도 다채웠다가 /usr/module에서 옛날커널 모듈 지움~~\
[수정패치 규칙](https://www.kernel.org/doc/html/v4.17/process/submitting-patches.html#diff-up)에 맞춰서 makefile 수정사항을 기록.\
```diff -u Makefile.orig Makefile > Makefile.patch```
## Assignment03
## Assignment04
## Assignment05
## Assignment06
## Assignment07
## Assignment08
## Assignment09