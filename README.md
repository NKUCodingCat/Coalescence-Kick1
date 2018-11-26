# Coalescence-Kick1
Unofficial Document for Coalescence-Kick

## Install

You can git clone this repository or Download zip & extract

Then simply run `gcc -lm PeriodicTable.c kick.c -o <executable_file_name>`

## Usage

A typical working directory structure could be like this

    Working_Dir
    ├── Executable                  # The executable you just compile
    └── INS                         # Instruction file, you can modify the origin one but keep the name 

Then run the executable(e.g. `.\kick.exe`)

Then you will find that the outputs is soooooooooooooooooooooooooooooooo long so I prefer to `.\<executable> > logs.txt`, you can easily debug your INS file by reading the text

It will generate a lots of gaussian input files if works perfectly, use Gaussian to run them. Here's a script to boost your workflow [Gaussian-Automator](https://github.com/NKUCodingCat/Gaussian-Automator)

# Detailed INS file format

For reference only, please read & debug the code when needed. Sample: [INS](./INS)

**I add indents when need, but please remember, _DO NOT_ leave any leading space in each line**


    title <title: string>
    gap <float>  
    box <x: float> <y: float> <z: float> 
    atoms  <Fragment or Atom: string> <Count: int> [ <Fragment or Atom: string> <Count: int> ... ]
    pop <int>
    restart <int #Seems unused>
    memory <MB: int> <Cores: int>
    header
    <Gaussian Header in one line, strat w/ #: str>
    <Charge: int> <Multiplicity: int>
    <Fragment: str>
    <Atom: int>      <x: float>    <y: float>    <z: float>
    [ 
        <Atom: int>      <x: float>    <y: float>    <z: float> 
        ... 
    ]
    [ 
        <Atom: int>      <x: float>    <y: float>    <z: float>
        [ 
            <Atom: int>      <x: float>    <y: float>    <z: float> 
            ... 
        ]
      ...
    ]


    [
        >TAIL <YOU CAN LEAVE COMMENT HERE>
        <Gaussian Input tails: string>
        >ENDTAIL <YOU CAN LEAVE COMMENT HERE>
    ]


-----
Ref: Averkiev, B. B. Ph.D. Thesis, Utah State University, Logan, UT, 2009.
