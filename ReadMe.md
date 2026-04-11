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
* [Minecraft is a trademark owned by Microsoft Corportation.](https://trademarks.justia.com/791/45/minecraft-79145431.html)
* The author of this project does not claim ownership of this source code.
* The author of this project has bought a Minecraft Java Edition license officially, therefore,
  the author of this project has got the Minecraft rd-132211's `client.jar` legally.
* This project shall never contain assets (for example, textures) from Mojang Studios' games.
  If the player wishes to experiment with this project with Mojang Studios' game assets,
  it is up to them to get assets **legally**.

## Changes
* Custom world format that looks actually like a format (Original rd-132211 has only `byte`s stored in)
    * Contains: Width, Height, Depth (`int`); `size_t` Size (unused)
* Using SDL3 instead of LWJGL2
* Fullscreen mode (F11 to toggle)

### Textures
* Storing Textures in a dictionary, can be found with a block's name (example: `Textures::blockTextures["Grass"]`)
    * Do note: Code is still using layers to render, which is, by the way, buggy.
* Block textures now stored separately and in Assets/Blocks/*.webp
* Now using WebP format instead on PNG (because WebP is just better).
* Using custom textures instead of original to avoid additional legal trouble.


## Discovered bugs (won't fix unless i suddenly come back to the experiment)
* Chunk's blocks can suddenly become grass depending by their placements (*absolutely weird*), and can temporarily revert back depending on your look angle (*even weirder*)
* You can sometimes <inline style="color: green;">mine/craft</inline> blocks through blocks (somewhat similar to [seamshots](https://wiki.portal2.sr/Seamshot)?)
* Block highlight is not rendering... Ugh.

</div>