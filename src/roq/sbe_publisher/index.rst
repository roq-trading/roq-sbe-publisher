.. _roq-sbe-publisher:

roq-sbe-publisher
=================


Purpose
-------

* Publish market data as SBE message to UDP/multicast 


Conda
-----

* :ref:`Using Conda <tutorial-conda>`

.. tab:: Install
  
  .. code-block:: bash
  
    $ conda install \
      --channel https://roq-trading.com/conda/stable \
      roq-sbe-publisher
  
.. tab:: Run
  
  .. code-block:: bash
  
    $ roq-sbe-publisher \
          --name publisher \
          --config_file test.toml \                                                                                           
          --local_interface 192.168.188.64 \                                                                                    
          --multicast_address_snapshot 224.1.1.1 \                                                                              
          --multicast_port_snapshot 1234 \                                                                                      
          --multicast_address_incremental 224.1.1.1 \                                                                           
          --multicast_port_incremental 6789 \                                                                                   
          --multicast_ttl 4 \                                                                                                   
          --multicast_loop=true \                                                                                               
          --snapshot_publish_freq 5s \                                                                                          
          ~/run/deribit.sock
  

Flags
-----

* :ref:`Using Flags <abseil-cpp>`

.. code-block:: bash

   $ roq-sbe-publisher --help

.. tab:: Flags

   .. include:: flags/flags.rstinc

.. tab:: Multicast

   .. include:: flags/multicast.rstinc


Config
------

The config file defines the subscriptions.

Symbols
~~~~~~~

Defined per exchange

.. code-block:: toml

   [symbols.deribit]
   regex = [ "BTC-PERPETUAL", "ETH-PERPETUAL" ]

Currencies
~~~~~~~~~~

Defined for all exchanges

.. code-block:: toml

   [symbols]
   regex = [ "USD.*", "BTC.*" ]
