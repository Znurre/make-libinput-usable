# make-libinput-usable

Have you also sat down at your Linux computer, wondering why your mouse is absolutely unusable? That no matter how you set the sliders in your mouse configuration UI, the acceleration feels absolutely terrible?

You are not alone, as a quick web search will tell you. There are tons of reports of the standard profile of libinput being less than ideal for *many people*, me included. 
Luckily, libinput has added support for custom acceleration profiles, which is supposedly able to fix this. There's even a [famous python script](https://gist.github.com/yinonburgansky/7be4d0489a0df8c06a923240b8eb0191) that 
can be used to get an acceleration curve for your mouse that is intended to emulate that of Microsoft Windows.

## The catch

Mouse acceleration is applied client side for `libinput`, and not all clients have UIs that allow you to set a custom acceleration profile.

What does this mean in practice?

You see, the client (a Wayland compositor, in most cases) will initiate libinput, and it will apply the profiles etc based on its own configuration. It will then start receiving events by using `libinput_dispatch`, and any
mouse events will have acceleration applied by libinput according to the active profile.

This means that, no matter how hard you try, you will not be able to affect the acceleration profile using libinput APIs from an external application, for example, because that configuration only affects the active context.

In my case, I am using KDE Plasma, which happens to not have support for the custom acceleration profile. There [have been attempts](https://invent.kde.org/plasma/kwin/-/merge_requests/6209) to introduce it, but it has
yet to be accepted.

I was not going to compile and maintain a downstream KWin version just so that I could set a custom acceleration profile, so I set out to look for alternative solutions.

## The solution

Turns out, it's way easier than one could think. We can simply use LD_PRELOAD to override the behavior of `libinput_device_config_accel_set_profile`, so that when KWin tries to apply the profile, we will override it with
our own configuration. Obviously, this is very crude, as it will apply for every single device on the system. You might want to extend the code to only apply it for specific devices, such as your mouse.

In order to apply this globally to every application that uses libinput, simply compile it, and then add the path to the library file to `/etc/ld.so.preload`

## Compiling

```
qmake
make
```

Why qmake? Because it's easy. You can probably quite easily compile it with `g++` directly as well.
