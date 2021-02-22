
Vue.component('play-button', {
  props: ["mode", "state"],
  methods: {
    clicked: function() {
      this.$emit('click', {event: this.mode});
    },
    css() {
      let result = "play-button";
      if (this.state == this.mode.name) {
        result += " player-active";
      }
      return result;
    }
  },
  template: `
    <div :class="css()">
      <img :src="'images/' + mode.name + '.png'" v-on:click="clicked">
    </div>
    `
});

Vue.component('play-controls', {
  mounted: function() {
    this.startRequesting();
  },
  beforeDestroy: function() {
    this.stopRequesting();
  },    
  data: function() {
    return {
      state: undefined,
      request_world: new PeriodicRequest(100, this.on_recv_world, this.on_recv_world_err),
      request_modes: new PeriodicRequest(100, this.on_recv_modes, this.on_recv_modes_err),
      world_data: undefined,
      modes_data: undefined
    }
  },  
  methods: {
    play: function(e) {
      this.state = e.event.name;
      app.put("player/" + this.state, (msg) => { });
    },
    startRequesting() {
      this.request_world.startRequesting("entity/flecs/core/World");
      this.request_modes.startRequesting("browse/flecs/player/mode?include=flecs.player.PlayerMode");
    },
    stopRequesting() {
      this.request_world.stopRequesting();
      this.request_modes.stopRequesting();
    },
    on_recv_world(data) {
      this.world_data = data;
      if (!this.state) {
        this.state = this.world_data.data["flecs.player.Player"].mode;
      }
    },
    on_recv_world_err() {
      this.state = undefined;
    } ,
    on_recv_modes(data) {
      this.modes_data = data;
    },
    on_recv_modes_err() {
      this.modes_data = undefined;
    }
  },
  template: `
    <div class="play-controls">
      <div class="play-buttons">
        <play-button v-for="mode in modes_data" :mode="mode" :state="state" v-on:click="play"></play-button>
      </div>
      <perf-summary :data="world_data"></perf-summary>
    </div>
    `
  });
