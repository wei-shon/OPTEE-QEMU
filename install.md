# Ubuntu 20.04 OPTEE INSTALL QEMU 3.12.0
###### tags: OPTEE install
## 0. 為何使用 Qemu?

OP-TEE 為運行在 TrustZone 上的 TEE (Trusted Execution Environment)
而 TrustZone 為基於 ARM 處理器架構的硬體安全技術，「樹莓派」即是 ARM 架構
OP-TEE 的開發團隊也有開發樹莓派的版本
為了日後在樹莓派上開發的方便，先使用 QEMU 虛擬化的技術
在 Linux 上模擬 ARM 架構的硬體環境，來進行 OP-TEE 的開發測試
待測試成功後再將環境移至樹莓派中
## 1. 建置 Ubuntu 環境
我使用 Ubuntu 20.04 作為我的環境

有以下兩種方案：
### (1) 安裝實體 Host Linux：

    製作 Ubuntu 20.04 USB 隨身碟
    安裝 Ubuntu 20.04 於實體硬碟

### (2) 虛擬機上安裝：

    可以使用 Virtualbox
    也可以使用VMware

## 2. 安裝 Qemu
### (1) 先檢查 CPU 是否有支援虛擬化，輸入以下指令

    $ sudo egrep "vmx|svm" /proc/cpuinfo

若有出現文字即表示有支援，反之則不支援，沒支援也沒有差別，就繼續往下做：

### (2) 安裝 Qemu 套件

    $ sudo apt-get update
    $ sudo apt-get install qemu
    $ sudo apt-get install qemu-kvm libvirt-bin bridge-utils virt-manager

### (3) 權限設定

使用 kvm 虛擬機時，必須是 root 用戶，或是 libvirtd 群組的成員
若使用 root 權限則可以跳過此步驟
如果沒有 root 權限，我們可以把自己的用戶加到 libvirtd 群組裡
以下的 <username> 請改成你自己的用戶名稱

    $ sudo adduser <username> libvirtd

### (4) 測試

先將虛擬機重新開機
重新開機後，在 command line 執行以下指令

    $ sudo virsh -c qemu:///system list

若出現以下畫面即安裝成功

    Id    Name                State
    ----------------------------------------

## 3. 下載並編譯 OP-TEE (以下內容參考此頁)
### (1) 安裝 OP-TEE 需要的套件

    $ sudo apt-get update
    $ sudo apt-get install android-tools-adb android-tools-fastboot autoconf \
       automake bc bison build-essential ccache cscope curl device-tree-compiler \
       expect flex ftp-upload gdisk iasl libattr1-dev libc6:i386 libcap-dev \
       libfdt-dev libftdi-dev libglib2.0-dev libhidapi-dev libncurses5-dev \
       libpixman-1-dev libssl-dev libstdc++6:i386 libtool libz1:i386 make \
       mtools netcat python-crypto python3-crypto python-pyelftools \
       python3-pycryptodome python3-pyelftools python-serial python3-serial \
       rsync unzip uuid-dev xdg-utils xterm xz-utils zlib1g-dev
    $ sudo apt-get install android-tools-adb android-tools-fastboot autoconf \
    automake bc bison build-essential ccache cscope curl device-tree-compiler \
    expect flex ftp-upload gdisk acpica-tools libattr1-dev libc6:i386 libcap-dev \
    libfdt-dev libftdi-dev libglib2.0-dev libhidapi-dev libncurses5-dev \
    libpixman-1-dev libssl-dev libstdc++6:i386 libtool zlib1g:i386 make \
    mtools netcat      python3-crypto python      python3-pycryptodome python3-pyelftools  python3-pip \
    python3-pycryptodome python3-pyelftools  python3-serial \
    rsync unzip uuid-dev xdg-utils xterm xz-utils zlib1g-dev


### (2) 安裝 repo

OP-TEE 使用 repo 來下載所有 source code，在你的資料夾下開始以下步驟

    $ mkdir ~/bin
    $ PATH=~/bin:$PATH
    $ curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
    $ chmod a+x ~/bin/repo

repo 需要你的 git 用戶名和 email，若沒有 github 帳號，先去創一個：

    $ git config --global user.name "yourname"        // 改成你的 git 名稱
    $ git config --global user.email "youremail"      // 改成你的 email

### (3) 使用 repo下載 OP-TEE source code

OP-TEE 的開發團隊為原始碼維護了許多版本 ( 2020/06/12更 )

支援的平台與版本可以到這個目錄看

在安裝的第三步驟，可以看到下面這行：

    $ repo init -u https://github.com/OP-TEE/manifest.git -m ${TARGET}.xml [-b ${BRANCH}]

其中 {TARGET}.xml 要指定安裝的平台，因為要裝到 QEMU 裡面，所以為 qemu_v8.xml。

[-b ${BRANCH}] 是選擇安裝的版本號碼用的，若沒指定的話預設就安裝 master。
### 下載原始碼

這邊我安裝的版本為 qemu_v8 的 3.12.0 版本
選好設定檔與版本號之後，創一個 OP-TEE 的資料夾，在你的資料夾中執行下列指令：

    $ mkdir optee              // 創建存放原始碼的資料夾，名稱可自訂，這邊我訂為 optee
    $ cd optee
    $ repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml -b 3.12.0
    $ repo sync -j$(nproc)     // 開始下載原始碼，-j$(nproc) 可以加快速度

### (4) 取得 toolchains

toolchains 是用來 Cross Compile 原始碼用的，執行以下指令來下載：

    $ cd ~/optee/build
    $ make -j2 toolchains

##### 若上面指令報錯的話先到這個網址裡面:    https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads/9-2-2019-12

##### 下載下面兩個名稱的檔案:
    gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf.tar.xz
    gcc-arm-9.2-2019.12-x86_64-aarch64_be-none-linux-gnu.tar.xz
    
先把optee裡面的toolchain資料夾中的檔案清空，再把這兩個檔案放toolchain資料夾中並解壓縮，然後再去執行一次上面的指令應該就可以成功安裝了



### (5) 開始編譯環境

準備好 source code 和 toolchains 後，開始編譯 OP-TEE 和 Qemu

    $ cd ~/optee/build
    $ make -j$(nproc) -f qemu_v8.mk all

mk 檔請對照安裝的 OP-TEE 版本

## 4. Qemu 中運行 OP-TEE
### (1) 啟動 Qemu

    $ cd ~/optee/build
    $ make -f qemu_v8.mk run-only

mk 檔是對照安裝的 OP-TEE 平台

可以看到額外開啟了兩個小黑窗，分別為 Normal world 和 Secure world

### (2) 啟動 REE 和 OP-TEE

在 Qemu 的小黑窗輸入 c，即可開啟兩個世界分別的 OS
圖上左邊視窗為 Normal world 的 REE，右邊視窗為 OP-TEE

### (3) 登入系統

在 Normal world 輸入 root 登入系統
先到根目錄 ls 一下，可以看到這就是 Normal world (Linux) 的檔案系統
是由 buildroot 這個工具建立起來的檔案系統

### (4) 查看 user command

    $ ls /usr/bin

可以看到下列各種 command，綠色字體的 command 跟 OP-TEE 有關

### (5) 運行 OP-TEE Example Application

上一步驟的綠色字體可以看到其中幾個就是 OP-TEE 的 Example Application
簡單選擇 hello_world 來運行一下：

    $ optee_example_hello_world

可以看到 Normal world 將一個數字 42 傳入 Secure world，Secure world 將 42 加一之後傳回來。
Secure world 的視窗可以看到一些 Debug 用的 log
Application 的細節可以去 Trace optee_examples 的原始碼

### (6) 運行 xtest

xtest 是 OP-TEE 用來檢測環境的程式

    $ xtest

若沒有 fail，表示 OP-TEE 有安裝成功且功能是完整的
### (7) 關閉 OP-TEE 與 Qemu

在 QEMU 的小黑窗輸入 q，即可關閉整個系統
