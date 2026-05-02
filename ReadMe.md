<div style="font-family: W95FA, system-ui; font-size: 110%;
background: url('Assets/Screenshot 2.webp'); background-size: cover; background-position: center;
border-radius: 20px; padding: 8pt;">
<h1 style="
    background: url('Assets/Screenshot.png'); background-size: cover; background-position: center;
    font-weight: 800; text-align: center; background-clip: text; color: transparent; text-shadow: none;
">cl_MC</h1>

Minecraft [rd-132211](https://minecraft.wiki/w/Java_Edition_pre-Classic_rd-132211), in C++.

**Status**: Fully ported, there's still typos/mistakes i might've made while porting.

## IMPORTANT LEGAL DISCLAIMER
* This project is not supported by [Mojang Studios](https://en.wikipedia.org/wiki/Mojang_Studios), Microsoft and Notch.
* This project is **only** for non-commercial use. You wouldn't want to get Mojang angry.
* This project was made for educational and recreational purposes.
* This project will not contain source codes from JE 1.0.0 (release) and JE post-1.0.0 (release).
* The author of this project is not affiliated with Mojang Studios, Microsoft and Notch.
* [Minecraft is a trademark owned by Microsoft Corporation.](https://trademarks.justia.com/791/45/minecraft-79145431.html)
* The author of this project does not claim ownership of this source code.
* The author of this project has bought a Minecraft Java Edition license officially, therefore,
  the author of this project has got the Minecraft rd-132211's `client.jar` legally.
* This project shall never contain assets (for example, textures) from Mojang Studios' games.
  The author of this project takes no responsibility for players using game assets
  from other games, while using this project.

## Changes
* Custom world format that looks actually like a format (Original rd-132211 has only `byte`s stored in)
    * Contains: Width, Height, Depth (`int`)
* Using SDL3 instead of LWJGL2 (though i'd like to see you use LWJGL in C++ :〉 )
* Fullscreen mode (F11 to toggle)
* Swapped actions for left and right mouse buttons

### Textures
* Now storing Textures in a dictionary, can be found with a block's name (example: `Textures::bindPRO("Grass")`)
* Block textures now stored separately and in Assets/Blocks/*.webp
* Now using WebP format instead of PNG (because WebP is just better).
* Using custom textures instead of original to avoid additional legal trouble.

## Coming soon...
* Rendering with Vulkan
* Native modding support

## Discovered bugs
* You can sometimes <inline style="color: green;">mine/craft</inline> blocks through blocks (somewhat similar to [seamshots](https://wiki.portal2.sr/Seamshot)?)

</div>