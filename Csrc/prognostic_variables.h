#pragma once
#include "grid.h"
#include<stdio.h>

struct VelocityDofs {
long u;
long v;
long w;
};

#pragma once
#include "grid.h"
#include<stdio.h>


void build_buffer(long nv, long dim, long s ,struct DimStruct *dims,
    double* restrict values, double* restrict  buffer){


    long i,j,k;

    const long istride = dims->nlg[1] * dims->nlg[2];
    const long jstride = dims->nlg[2];
    long var_shift =  dims->npg * nv ;
    long buffer_var_shift ,b_istride, b_jstride, b_ishift, b_jshift;
    long shift_offset ;

    if (dim == 0){
        if( s == 1){
           // The shift offset insures that correct values are build into buffer for positive (s = 1) and
           // negative (s = -1) MPI SendRecv shifts.
           shift_offset = dims->nlg[0]-2*dims->gw;
        }
        else{
           shift_offset = dims->gw;
        }
        b_istride = istride;
        b_jstride = jstride;
        buffer_var_shift = dims->nbuffer[0] * nv ;
        for (i=0; i<dims->gw; i++){
            const long ishift = (i + shift_offset) * istride ;
            b_ishift = i * b_istride;
            for (j=0; j<dims->nlg[1];j++){
                const long jshift = j * jstride ;
                b_jshift = j * b_jstride;
                for (k=0;k<dims->nlg[2];k++){
                    buffer[buffer_var_shift + b_ishift + b_jshift + k] = values[var_shift + ishift + jshift + k];
                };
            };
        };


        return;
    }
    else if (dim == 1){
        if( s == 1){
           // The shift offset insures that correct values are build into buffer for positive(s = 1) and
           // negative (s = -1) MPI SendRecv shifts.
           shift_offset = dims->nlg[1] - 2*dims->gw;
        }
        else{
            shift_offset = dims->gw;
        }
        //Compute the strides in i and j for the buffer arrays
        b_istride = dims->gw * dims->nlg[2];
        b_jstride = dims->nlg[2];

        //Compute the variable array shift
        buffer_var_shift = dims->nbuffer[1] * nv ;


        for (i=0; i<dims->nlg[0]; i++){
            const long ishift = i * istride;
            b_ishift = i * b_istride;
            for (j=0; j<dims->gw;j++){
                const long jshift = (j + shift_offset) * jstride;
                b_jshift = j * b_jstride;
                for (k=0;k<dims->nlg[2];k++){
                    buffer[buffer_var_shift + b_ishift + b_jshift + k] = values[var_shift + ishift + jshift + k];
                };
            };
        };

        return;
    }
    else {
        if(s == 1){
           // The shift offset insures that correct values are build into buffer for positive (s = 1) and
           // negative (s = -1) MPI SendRecv shifts.
           shift_offset = dims->nlg[2]-2*dims->gw;
        }
        else{
           shift_offset = dims->gw;
        }
        b_istride = dims->gw * dims->nlg[1];
        b_jstride = dims->gw;
        buffer_var_shift = dims->nbuffer[2] * nv ;
        for (i=0; i<dims->nlg[0]; i++){
            const long ishift = i * istride;
            b_ishift = i * b_istride;
            for (j=0; j<dims->nlg[1];j++){
                const long jshift = j * jstride;
                b_jshift = j * b_jstride;
                for (k=0;k<dims->gw;k++){
                    buffer[buffer_var_shift + b_ishift + b_jshift + k] = values[var_shift + ishift + jshift + k + shift_offset];
                };
            };
        };


        return;
    }


}

void buffer_to_values(size_t dim, size_t s, struct DimStruct *dims,
    double* restrict values, double* restrict buffer){

    size_t i,j,k;

    size_t istride = dims->nlg[1] * dims->nlg[2];
    size_t jstride = dims->nlg[2];
    size_t buffer_var_shift ;
    size_t shift_offset;

    if (dim == 0){
        if( s == -1){
           // The shift offset insures that correct values are build into buffer for positive (s = 1) and
           // negative (s = -1) MPI SendRecv shifts.
           shift_offset = dims->nlg[0] - dims->gw;
        }
        else{
           shift_offset = 0;
        }

        const size_t b_istride = dims->nlg[1] * dims->nlg[2];
        const size_t b_jstride = dims->nlg[2];

        for (i=0; i<dims->gw; i++){
            const size_t ishift =(i+shift_offset)*istride;
            const size_t b_ishift = i*b_istride;
            for (j=0; j<dims->nlg[1]; j++){
                const size_t jshift = j*jstride ;
                const size_t b_jshift = j*b_jstride;
                for (k=0; k<dims->nlg[2]; k++){
                    values[ishift + jshift + k] =  buffer[b_ishift + b_jshift +k];
                }
            }

        }

        return;
    }
    else if (dim == 1){
        if( s == -1){
           // The shift offset insures that correct values are build into buffer for positive(s = 1) and
           // negative (s = -1) MPI SendRecv shifts.
           shift_offset = dims->nlg[1] - dims->gw;
        }
        else{
            shift_offset = 0;
        }

        const size_t b_istride = dims->gw * dims->nlg[2];
        const size_t b_jstride = dims->nlg[2];
        for (i=0; i<dims->nlg[0]; i++){
            const size_t ishift = i * istride;
            const size_t b_ishift = i * b_istride;
            for (j=0; j<dims->gw;j++){
                const size_t jshift = (j + shift_offset) * jstride;
                const size_t b_jshift = j * b_jstride;
                for (k=0;k<dims->nlg[2];k++){
                     values[ishift + jshift + k] =  buffer[b_ishift + b_jshift + k] ;
                };
            };
        };

        return;
    }
    else {
        if(s == -1){
           // The shift offset insures that correct values are build into buffer for positive (s = 1) and
           // negative (s = -1) MPI SendRecv shifts.
           shift_offset = dims->nlg[2] - dims->gw;
        }
        else{
           shift_offset = 0;
        }
        const size_t b_istride = dims->gw * dims->nlg[1];
        const size_t b_jstride = dims->gw;
        for (i=0; i<dims->nlg[0]; i++){
            const size_t ishift = i * istride;
            const size_t b_ishift = i * b_istride;
            for (j=0; j<dims->nlg[1];j++){
                const size_t jshift = j * jstride;
                const size_t b_jshift = j * b_jstride;
                for (k=0;k<dims->gw;k++){
                    values[ishift + jshift + k + shift_offset] = buffer[b_ishift + b_jshift + k ];
                };
            };
        };


        return;
    };

    return;
}

void set_bcs(size_t dim, size_t s, double bc_factor ,struct DimStruct *dims,
    double* restrict values){

        size_t i,j,k;

        const size_t istride = dims->nlg[1] * dims->nlg[2];
        const size_t jstride = dims->nlg[2];

        if(dim==2){
            if(s==-1){
                const size_t bc_start = dims->nlg[2] - dims->gw;  // This is the index of the first boundary point
                for (i=0; i<dims->nlg[0]; i++){
                    const size_t ishift = i * istride;
                    for(j=0;j<dims->nlg[1];j++){
                        const size_t jshift = j * jstride;
                        if(bc_factor == 2.0){
                            for(k=0;k<dims->gw;k++){
                                values[ishift + jshift + bc_start +k  ] = values[ishift + jshift + bc_start  - k  -1 ];
                                }
                        }
                        else if(bc_factor == 1.0){
                            for(k=0;k<dims->gw;k++){
                                values[ishift + jshift + bc_start +k  ] = values[ishift + jshift + bc_start -1 ];
                             }
                        }
                        else{
                            values[ishift + jshift + bc_start ] = 0.0;
                            for(k=1;k<dims->gw;k++){
                                values[ishift + jshift + bc_start + k] = bc_factor * values[ishift + jshift +  bc_start - k ];
                            }
                        }
                    }
                }

            }
            else{
                const size_t bc_start = dims->gw-1;
                for (i=0; i<dims->nlg[0]; i++){
                    const size_t ishift = i * istride;
                    for(j=0;j<dims->nlg[1];j++){
                        const size_t jshift = j * jstride;
                        if(bc_factor == 2.0){
                            for(k=0;k<dims->gw;k++){
                                values[ishift + jshift + bc_start -k ] = values[ishift + jshift + bc_start  + k + 1];
                             }
                        }
                        else if(bc_factor == 1.0){
                            for(k=0;k<dims->gw;k++){
                                values[ishift + jshift + bc_start -k ] = values[ishift + jshift + bc_start + 1];
                            }
                        }
                        else{
                            values[ishift + jshift + bc_start ] = 0.0;
                            for(k=1;k<dims->gw;k++){
                                values[ishift + jshift + bc_start - k] = bc_factor * values[ishift + jshift + bc_start +k ];
                            }
                        }
                    }
                }
            }
        }
        else{
            printf("%s\n","PrognocitVariables.set_bcs only implemented for dim=2");
        }
    return;
}


void set_to_zero(size_t nv, struct DimStruct *dims, double* restrict array){

    size_t i;
    for (i = 0; i<dims->npg*nv; i++){
        array[i] = 0;
    };
    return;
}
