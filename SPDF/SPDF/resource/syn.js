import {syn_SPOL} from "./__story__/syn_SPOL.js"
function main(){
    SPOL.loadSPOL(syn_SPOL.PartA)
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
    
