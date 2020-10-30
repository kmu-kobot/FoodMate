---
description: How to build (SW)
---

# SW

## 1. 전제 조건 및 종속성

Jeston 장치에 Tensorflow를 설치하기 위해서는 다음을 확인하세요

1. Jetson 장치에 [JetPack](https://developer.nvidia.com/jetpack)을 설치합니다.
2. TensorFlow에 필요한 시스템 패키지를 설치합니다.

   ```text
   $ sudo apt-get update
   $ sudo apt-get install libhdf5-serial-dev hdf5-tools libhdf5-dev zlib1g-dev zip libjpeg8-dev liblapack-dev libblas-dev gfortran
   ```

3. pip 설치 및 업그레이드

   ```text
   $ sudo apt-get install python3-pip
   $ sudo pip3 install -U pip testresources setuptools==49.6.0
   ```

4. Python 패키지 종속성을 설치합니다.

   ```text
   $ sudo pip3 install -U numpy==1.16.1 future==0.18.2 mock==3.0.5 h5py==2.10.0 keras_preprocessing==1.1.1 keras_applications==1.0.8 gast==0.2.2 futures protobuf pybind11
   ```

## 2.Tensorflow 설







Becoming a super hero is a fairly straight forward process:

```
$ give me super-powers
```

{% hint style="info" %}
 Super-powers are granted randomly so please submit an issue if you're not happy with yours.
{% endhint %}

Once you're strong enough, save the world:

{% code title="hello.sh" %}
```bash
# Ain't no code for that yet, sorry
echo 'You got to trust me on this, I saved the world'
```
{% endcode %}



