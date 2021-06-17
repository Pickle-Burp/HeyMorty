# HeyMorty !

How to use:

```sh
./hey_morty <audio/cmd/ui/wk>
./hey_morty audio <audio_file> # Perform the audio recognition
./hey_morty cmd <a plain text sentence> # Perform a given task
./hey_morty ui # Launch the user interface
./hey_morty wk <audio_file> # Perform the audio recognition and execute the command in the audio
```

## Example :
Create a audio file containing your sentence in single channel, 16kHz.
Possible actions : 
> \<\> : Must be filled  
> \[\] : optional
- Ouvre \<application\>
  > Ex: Ouvre firefox
- Recherche \<something to search\> \[sur \<browser\>\]
  > Ex: Recherche comment faire des cookies
  >     Recherche des maisons sur brave

Then, parse the audio with this command: 
```sh
./hey_morty wk "src/assets/ouvre spotify.wav"
```
The project will open spotify if installed on your machine !!

## Libraries :

- libcurl4-openssl-dev
- libflac-dev
- libasound-dev
