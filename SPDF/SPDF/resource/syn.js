import {syn_SPOL} from "./__story__/syn_SPOL.js"
export function main(){
    SPDF.loadStory(syn_SPOL.PartA)
    // var master_love_index = SPDF.branch(["选项一","选项二","选项三"])
    // if (master_love_index == 0){
    //     SPDF.loadSPOL(syn_SPOL.PartB)
    // }else{
    //     SPDF.loadSPOL(syn_SPOL.PartC)
    // }
    // master_love_index += SPDF.branch(["选项一","选项二","选项三"])
    // if (master_love_index>5){
    //     SPDF.loadSPOL(syn_SPOL.PartGoodEnd)
    // }else{
    //     SPDF.loadSPOL(syn_SPOL.PartBadEnd)
    // }
}

/*SPOL0.10.0
-->PartA
SWITCH 2
    CASE 1
        >>>:"Hello World 1"
    CASE 2
        >>>:"Hello World 2"
        SWITCH 5
            CASE 4
                >>>:"Hello World 2-4"
            CASE 5
                >>>:"Hello World 2-5"
                SWITCH 1
                    CASE 1
                        >>>:"Hello World 2-5-1"
                    CASE 2
                        >>>:"Hello World 2-5-2"
    CASE 3
        >>>:"Hello World 3"
>>>:"Hello World 0"

SPOL*/
    
