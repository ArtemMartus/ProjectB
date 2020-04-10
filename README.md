# ProjectB [MacOS/Ubuntu/Win]
<h2>About</h2>
<p>This is multiplayer chess game with all rules implemented (including castling/ checkmating).</p>
<p>When you start an app you'll see following window: <img src='/screenshots/first.png'/></p>
<p>In here we can see the chessboard with coordinates on edges and already placed figures on it.</p>
<p>White figures rendered as capital letters where <b>P</b> = <b>P</b>awn, <b>R</b> = <b>R</b>ook, <b>N</b> = K<b>N</b>ight, <b>B</b> = <b>B</b>ishop, <b>Q</b> = <b>Q</b>ueen, <b>K</b> = <b>K</b>ing</p>
<b>Type</b><ol start="0">
  <li>to move</li>
  <li>to save the game</li>
  <li>to load the game</li>
  <li>to surrender and quit</li>
</ol>
<p>Press 0 and you're prompted to type in one of suggested coordinates of an ally figure and then type to coordinate of where you want it to move. <img src="/screenshots/second.png"/></p>
<p>After successful move you'll see a message and new checkboard with moved unit. <img src="/screenshots/fourth.png"/></p>
<hr>
<h2>Build using CMake</h2>

<ul type="circle">
  <li><b>MacOs</b> - no dependencies</li>
  <li><b>Ubuntu:</b> - no dependencies</li>
  <li><b>Win</b> - no dependencies</li>
</ul>
<p><code>cmake . && make</code></p>
<h2>Tests can be run via CTest</h2>
<p>Run all tests with command <code>make test</code> or <code>ctest</code> after building whole project</p>

<img src='/screenshots/valgrind.png'/>
