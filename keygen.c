#include <openssl/bn.h>
#include <openssl/dh.h>


DH *get_dh1024()
{
    static unsigned char dhp_1024[] = {
	0xE2, 0x7B, 0x75, 0x52, 0x46, 0xCA, 0xA0, 0x1F, 0xE6, 0xC6, 
	0xC2, 0xDF, 0x3D, 0x5A, 0xBB, 0x52, 0x28, 0xAC, 0xE8, 0x4C, 
	0x90, 0xE1, 0x4B, 0x56, 0x24, 0x10, 0x02, 0xC8, 0xBE, 0xE9, 
	0x89, 0xA5, 0xDC, 0xB1, 0xBE, 0x0A, 0x55, 0x0D, 0x6C, 0xE0, 
	0xA1, 0x24, 0xE3, 0x18, 0x2B, 0xAE, 0x75, 0x91, 0xA9, 0xF1, 
	0x4C, 0xC1, 0x81, 0xA5, 0x57, 0x8E, 0x72, 0x5D, 0x76, 0x5D, 
	0xF6, 0x7C, 0x3E, 0x76, 0xA7, 0x5F, 0x4D, 0x7A, 0xF7, 0xC2, 
	0xBE, 0xC1, 0x42, 0x59, 0x8C, 0xD3, 0xF4, 0x71, 0x77, 0xF5, 
	0xA6, 0xAB, 0x82, 0xA8, 0xAA, 0x3A, 0xFB, 0x62, 0x3B, 0x26, 
	0xBE, 0x4B, 0xE1, 0xED, 0xB1, 0x39, 0x3E, 0x28, 0xD6, 0x73, 
	0xD2, 0x55, 0x52, 0xF8, 0xEF, 0xB3, 0x59, 0x04, 0x5F, 0x80, 
	0x85, 0x07, 0xAD, 0xFC, 0x12, 0xEC, 0xDF, 0x7C, 0xC9, 0xD1, 
	0x2B, 0x1E, 0x3A, 0x62, 0x6E, 0xC6, 0x72, 0x0F
    };
    static unsigned char dhg_1024[] = {
	0x05
    };
    DH *dh = DH_new();
    BIGNUM *dhp_bn, *dhg_bn;

    if (dh == NULL)
        return NULL;
    dhp_bn = BN_bin2bn(dhp_1024, sizeof (dhp_1024), NULL);
    dhg_bn = BN_bin2bn(dhg_1024, sizeof (dhg_1024), NULL);
    if (dhp_bn == NULL || dhg_bn == NULL
            || !DH_set0_pqg(dh, dhp_bn, NULL, dhg_bn)) {
        DH_free(dh);
        BN_free(dhp_bn);
        BN_free(dhg_bn);
        return NULL;
    }
    return dh;
}

int main() {

	DH *dh;
	FILE *f;
	BIGNUM *gtox,*p,*q,*g,*priv_key,*pub_key;
	BN_CTX *ctx;

	gtox=BN_new();
	ctx = BN_CTX_new();
	dh=get_dh1024();
	DH_get0_pqg(dh,&p,&q,&g);
	printf("Prime: %s \n\n",BN_bn2dec(p));
	DH_generate_key(dh);
	DH_get0_key(dh,&pub_key,&priv_key);
	printf("Private Key: %s \n\n",BN_bn2dec(priv_key));
	printf("Public Key: %s \n\n",BN_bn2dec(pub_key));

	// Doublecheck the public key. Effectively   g^x
	BN_mod_exp(gtox, g, priv_key, p, ctx);
	printf("Public Key (gtox): %s \n\n",BN_bn2dec(gtox));
}

