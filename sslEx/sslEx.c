/*
*   This file is a first attemp to develope secure apps using openssl.
*   Tutorial on: https://developer.ibm.com/technologies/security/tutorials/l-openssl/
*/
#include <stdio.h>
#include <stdlib.h>

/* Minimal OpenSSL necessary headers */
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

int main(int argc, char **argv)
{
    /* Initialize OpenSSL */
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    /* BIO: abstraction library for handling connections of various types (including files and sockets) whether secure or not. */
    BIO *bio;

    /* 
    *   Open a new connection with BIO. You can directly pass the host and port to the BIO_new_connect, or pass firts 
    *   the host, and then the port, with any of the following functions: BIO_set_conn_port, BIO_set_conn_int_port.
    *   BIO will return NULL when the connection could not be done.
    */
    bio = BIO_new_connect("127.0.0.1:8080");
    if (bio == NULL)
    {
        /* Handle the failure */
        ;
    }

    /*
    *    Call BIO_do_connect to verify that the connetion was made.
    */
    if (BIO_do_connect(bio) <= 0)
    {
        /* Handle failed connection */
        ;
    }

    /*
    *   Reading and writing is done with the functions: BIO_read and BIO_write, regardless of the type of file object to be read from.
    *   BIO_read attemps to read a certain number of bytes from the file descriptor. It returns the number of bytes read, 0 when the 
    *       connection was closed or no bytes were available (non-blocking connection), and -1 when an error ocurred (?).
    *   BIO_write: attemps to write bytes to socket. Returns the number of bytes actually written, 0 or -1, which may not indicate errors.
    *       Use BIO_should_retry to find out the reason.
    *   BIO_should_retry: determine the error occurred.
    */
    int x = BIO_read(bio, buf, len);
    if (x == 0)
    {
        /* Handle closed connection */
    }
    else if (x < 0)
    {
        if (!BIO_should_retry(bio))
        {
            /* Handle failed read here */
        }
        
        /* Do something to handle the retry */
    }

    /* Writing */
    ig (BIO_write(bio, buf, len) <= 0)
    {
        if (!BIO_should_retry(bio))
        {
            /* Handle failure here */
        }

        /* Do something to handle the retry */
    }

    /*
    *   Closing a connection. For closing a connection, one of two functions maybe used: 
    *   BIO_reset: This function closes the connection, and then reset the BIO internal state, so that the connection can be reused.
    *   BIO_free_all: Frees the internal structure and releases all associated memory, closing the associated socket.
    */

    /* Closing and reusing */
    BIO_reset(bio);

    /* Closing and freeing */
    BIO_free_all(bio);

    /*
    *   Secure Connection. A secure connection requires a handshake, during which the server sends a certificate to the client. Then the
    *   client verifies it agains a set of trusted certificates, and checks whether it has expired or not. RFC 2246.
    *   
    *   For a secure connection, a pointer to the type SSL_CTX data structure is required. Such data structure holds SSL information, and is
    *   used to set up the SSL connection via the BIO library.
    */
   SSL_CTX ctx = SSL_CTX_new(SSLv23_client_method());

    /*
    *    Another pointer is needed to hold the SSL connection strcucture, and can be used to examine the connection information or set up
    *    addtional SSL parameters.
    */
    SSL ssl;

    /*
    *   Trusted certificate.
    *   A certifcate is used for the verification of the peer. If no certificate is given, the certificate is marked as invalid, but the connection can keep on.
    *   OpenSSL already provides a set of trust certificates, which are stored in a file called "TrustStore.pem".
    *   For loading such set, the function SSL_CTX_load_verify_locations must be called.
    */
    if (!SSL_CTX_load_verify_locations(ctx, "path/to/file.pem", NULL))
    {
        /* Handle failure here */
    }

    /* 
    *   A command include within the OpenSSL command line tool can be used to set a directory as Trust Store folder:
    *   c_rehash /path/to/folder
    *   Which can be called from within the code:
    */
    if (!SSL_CTX_load_verify_locations(ctx, NULL, "path/to/folder"))
    {
        /* Handle errors here */
    }

    /*
    *   Creating a secure connection object.
    *   To set a secure connection, the function BIO_new_ssl_connect is used, recieving only the ssl ctx structure.
    *   The function SSL_set_mode function can be used to set the mode parameter of the ssl structure. The option SET_MODE_AUTO_RETRY
    *   can be used to tell the program to handle new handshakes.
    */
    BIO bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    /*
    *   After this, call the previous mentioned functions to create and verify the connection.
    */

    /*
    *   The validity of the certificate must still be checked. This can be done using the function SSL_get_verify_result with the ssl data
    *   structure as the only parameter. It should return X509_V_OK when everything was fine. If it fails, the connection can still be used, 
    *   but additional security considerations may be taken.
    */
    if (SSL_get_verify_result(ssl) != X509_V_OK)
    {
        /* Handle the failed verification */
    }

    /*
    *   After this, the object bio can be used as previously stated. When all is done, the ctx data structure must be freed.
    *   This si done using the function SSL_CTX_free.
    */
   SSL_CTX_free(ctx);

    return 0;
}
